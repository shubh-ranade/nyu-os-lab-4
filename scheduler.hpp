#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "io_request.hpp"
#include <queue>
#include <list>

class Scheduler {
public:
    virtual bool has_more_io() = 0;
    virtual IORequest* get_next_io() = 0;
    virtual void add_io(IORequest*) = 0;
};

class FIFOSched : public Scheduler {
private:
    std::queue<IORequest*> q;
public:
    FIFOSched() = default;
    bool has_more_io();
    IORequest* get_next_io();
    void add_io(IORequest*);
};

class SSTFSched : public Scheduler {
private:
    std::list<IORequest*> q;
public:
    SSTFSched() = default;
    bool has_more_io();
    IORequest* get_next_io();
    void add_io(IORequest*);
};

class LOOKSched : public Scheduler {
private:
    std::list<IORequest*> q;
public:
    LOOKSched() = default;
    bool has_more_io();
    IORequest* get_next_io();
    void add_io(IORequest*);
};

#endif