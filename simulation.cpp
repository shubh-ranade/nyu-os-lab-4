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

int main(int argc, char** argv) {

    // handle arguments with getopt
    int vflag = 0, fflag = 0, qflag = 0;
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
    while(get_next_request(input, &at, &tr))
        reqlist.push_back(new IORequest(at, tr));
    input.close();

    // sanity check passing
    // int i = 0;
    // for(auto req : reqlist)
    //     cout << "Requst #" << i++ << ": " << req->getAT() << ' ' << req->getTR() << endl;

    Scheduler* sched;

    return EXIT_SUCCESS;
}
