#ifndef IO_REQUEST_HPP
#define IO_REQUEST_HPP

class IORequest {
private:
    unsigned int op_num;
    unsigned int arrival_time, track_requested, start, end;
public:
    IORequest(int op, int at, int tr) : op_num(op), arrival_time(at), track_requested(tr), start(0), end(0) {}
    unsigned int getAT() { return arrival_time; }
    unsigned int getTR() { return track_requested; }
    unsigned int getStartTime() { return start; }
    unsigned int getEndTime() { return end; }
    unsigned int getOpNum() { return op_num; }
    void setStartTime(int s) { start = s; }
    void setEndTIme(int e) { end = e; }
};

#endif