#include "scheduler.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

extern int disk_head;
extern unsigned int qflag;

bool FIFOSched::has_more_io() {
    return !q.empty();
}

IORequest* FIFOSched::get_next_io() {
    if(q.empty()) return nullptr;

    IORequest* ret = q.front();
    q.pop();
    return ret;
}

void FIFOSched::add_io(IORequest* r) {
    q.push(r);
}

bool SSTFSched::has_more_io() {
    return !q.empty();
}

IORequest* SSTFSched::get_next_io() {
    int min_seek_time = __INT32_MAX__;
    IORequest* ret = nullptr;
    list<IORequest*>::iterator remove;
    if(q.empty()) return ret;

    if(qflag) printf("\t");
    for(list<IORequest*>::iterator it = q.begin(); it != q.end(); it++) {
        IORequest* req = *it;
        int seek_time = abs(req->getTR() - disk_head);
        if(qflag) printf("%u:%u ", req->getOpNum(), seek_time);
        if(seek_time < min_seek_time) {
            min_seek_time = seek_time;
            ret = req;
            remove = it;
        }
    }
    if(qflag) printf("\n");

    if(ret) q.erase(remove);

    return ret;
}

void SSTFSched::add_io(IORequest* r) {
    q.push_back(r);
}
