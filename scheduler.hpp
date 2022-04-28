#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "io_request.hpp"

class Scheduler {
public:
    virtual bool has_more_io() = 0;
    virtual IORequest get_next_io() = 0;
    virtual void add_io() = 0;
};

#endif