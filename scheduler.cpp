#include "scheduler.hpp"

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
