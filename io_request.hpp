#ifndef IO_REQUEST_HPP
#define IO_REQUEST_HPP

class IORequest {
private:
    unsigned int arrival_time, track_requested;
public:
    IORequest(int at, int tr) : arrival_time(at), track_requested(tr) {}
    unsigned int getAT() { return arrival_time; }
    unsigned int getTR() { return track_requested; }
};

#endif