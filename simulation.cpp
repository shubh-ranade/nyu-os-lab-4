#include "io_request.hpp"
#include "scheduler.hpp"
#include "utils.hpp"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

int disk_head, direction;
unsigned int vflag = 0, fflag = 0, qflag = 0;

int main(int argc, char** argv) {

    // handle arguments with getopt
    char *spec = NULL;
    int index;
    int c;
    char sched_algo;

    opterr = 0;

    while ((c = getopt (argc, argv, "s:vqf")) != -1)
    switch (c)
    {
    case 's':
        spec = optarg;
        sscanf(spec, "%c", &sched_algo);
        break;
    case 'v':
        vflag = 1;
        printf("TRACE\n");
        break;
    case 'q':
        qflag = 1;
        break;
    case 'f':
        fflag = 1;

    case '?':
        if (optopt == 's')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
            fprintf (stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
        exit(EXIT_FAILURE);
    
    default:
        printf("ERROR opt %c\n", c);
        exit(EXIT_FAILURE);
    }

    if(!argv[optind]) {
		printf("Not enough non-optional arguments. Got %s\n", argv[optind]);
		printf("Usage: ./iosched [ -s<schedalgo> | -v | -q | -f ] <inputfile>\n");
	}

    // read input
    ifstream input;
    input.open(argv[optind]);
    list<IORequest*> reqlist;
    unsigned int at, tr;
    unsigned int opnum = 0;
    while(get_next_request(input, &at, &tr))
        reqlist.push_back(new IORequest(opnum++, at, tr));
    input.close();

    vector<IORequest*> finishedlist(opnum);

    // sanity check passing
    // int i = 0;
    // for(auto req : reqlist)
    //     cout << "Requst #" << i++ << ": " << req->getAT() << ' ' << req->getTR() << endl;

    Scheduler* sched;
    switch (sched_algo)
    {
    case 'i':
        sched = new FIFOSched();
        break;

    case 'j':
        sched = new SSTFSched();
        break;

    case 's':
        sched = new LOOKSched(false);
        break;
    
    case 'c':
        sched = new LOOKSched(true);
        break;

    case 'f':
        sched = new FLOOKSched();
        break;
    
    default:
        printf("Scheduler algo %c not supported\n", sched_algo);
        exit(EXIT_FAILURE);
    }

    unsigned int sim_time = 0;
    disk_head = 0;
    direction = 0; // 0: no movement 1: right -1: left
    IORequest* curr_io = nullptr;

    // stats
    unsigned int tot_movement = 0, max_waittime = 0;
    double tot_waittime = 0.0, tot_turnaround = 0.0;

    while(true) {        
        // cout << "sim time " << sim_time << " disk head " << disk_head << " tr ";
        // if(curr_io) cout << curr_io->getTR();
        // else cout << "N/A";
        // cout << endl;

        // if a new io request arrived at this time
        if(!reqlist.empty() && reqlist.front()->getAT() == sim_time) {
            IORequest* new_req = reqlist.front();
            sched->add_io(new_req);
            if(vflag) printf("%u: %5u add %u\n", sim_time, new_req->getOpNum(), new_req->getTR());
            reqlist.pop_front();
        }
        // if an active IO finished at this time
        if(curr_io != nullptr && disk_head == curr_io->getTR()) {
            // compute relevant info and store
            curr_io->setEndTIme(sim_time);
            if(vflag) printf("%u: %5u finish %d\n", sim_time, curr_io->getOpNum(), sim_time - curr_io->getAT());
            finishedlist[curr_io->getOpNum()] = curr_io;
            tot_movement += (int) curr_io->getEndTime() - (int) curr_io->getStartTime();
            curr_io = nullptr;
        }
        // if no request is active at this time
        if(curr_io == nullptr) {
            if(sched->has_more_io()) {
                // TODO: initialize accounting variables
                curr_io = sched->get_next_io();
                while(curr_io && curr_io->getTR() == disk_head) {
                    curr_io->setStartTime(sim_time);
                    if(vflag) printf("%u: %5u issue %u %u\n", sim_time, curr_io->getOpNum(), curr_io->getTR(), disk_head);
                    curr_io->setEndTIme(sim_time);
                    if(vflag) printf("%u: %5u finish %d\n", sim_time, curr_io->getOpNum(), sim_time - curr_io->getAT());
                    finishedlist[curr_io->getOpNum()] = curr_io;
                    curr_io = nullptr;
                    if(sched->has_more_io()) {
                        curr_io = sched->get_next_io();
                    }
                }
                if(curr_io) {
                    if(vflag) printf("%u: %5u issue %u %u\n", sim_time, curr_io->getOpNum(), curr_io->getTR(), disk_head);
                    direction = curr_io->getTR() > disk_head ? 1 : -1;
                    curr_io->setStartTime(sim_time);
                }
            }
            else if(reqlist.empty()) {
                // TODO: exit, print final info etc
                break;
            }
            else {
                direction = 0;
            }
        }
        // if there is an ongoing request
        disk_head += direction ? (direction / abs(direction)) : 0;

        sim_time++;
    }

    opnum = 0;
    for(auto r : finishedlist) {
        printf("%5d: %5d %5d %5d\n", opnum++, r->getAT(), r->getStartTime(), r->getEndTime());
        double curr_waittime = (int) r->getStartTime() - (int) r->getAT();
        tot_waittime += curr_waittime;
        tot_turnaround += (int) r->getEndTime() - (int) r->getAT();
        if(curr_waittime > max_waittime) max_waittime = curr_waittime;
    }

    double avg_turnaround = (double) tot_turnaround / opnum;
    double avg_waittime = (double) tot_waittime / opnum;

    printf("SUM: %u %u %.2lf %.2lf %u\n", sim_time, tot_movement, avg_turnaround, avg_waittime, max_waittime);

    return EXIT_SUCCESS;
}
