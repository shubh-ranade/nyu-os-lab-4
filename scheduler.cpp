#include "scheduler.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

extern int disk_head;
extern int direction;
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
        if(seek_time == 0) {
            ret = req;
            remove = it;
            break;
        }
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

bool LOOKSched::has_more_io() {
    return !q.empty();
}

IORequest* LOOKSched::get_next_io() {
    IORequest* ret = nullptr;
    if(q.empty()) return ret;

    int global_min_seek_time = __INT32_MAX__, same_dir_min_seek_time = __INT32_MAX__;
    list<IORequest*>::iterator global_min = q.end(), same_dir_min = q.end();
    for(auto it = q.begin(); it != q.end(); it++) {
        IORequest* req = *it;
        int seek_time = req->getTR() - disk_head;
        if(seek_time == 0) {
            ret = req;
            q.erase(it);
            return ret;
        }
        if(seek_time * direction > 0 && abs(seek_time) < same_dir_min_seek_time) {
            same_dir_min_seek_time = abs(seek_time);
            same_dir_min = it;
        }
        if(abs(seek_time) < global_min_seek_time) {
            global_min_seek_time = abs(seek_time);
            global_min = it;
        }
    }

    if(global_min != q.end()) ret = *global_min;
    if(same_dir_min != q.end()) ret = *same_dir_min;
    
    if(ret && ret == *global_min) q.erase(global_min);
    else if(ret && ret == *same_dir_min) q.erase(same_dir_min);

    return ret;
}

void LOOKSched::add_io(IORequest* r) {
    q.push_back(r);
}
