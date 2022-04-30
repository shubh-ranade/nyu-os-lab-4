#ifndef IO_REQUEST_HPP
#define IO_REQUEST_HPP

class IORequest {
private:
    unsigned int op_num;
    int arrival_time, track_requested, start, end;
public:
    IORequest(int op, int at, int tr) : op_num(op), arrival_time(at), track_requested(tr), start(0), end(0) {}
    int getAT() { return arrival_time; }
    int getTR() { return track_requested; }
    int getStartTime() { return start; }
    int getEndTime() { return end; }
    unsigned int getOpNum() { return op_num; }
    void setStartTime(int s) { start = s; }
    void setEndTIme(int e) { end = e; }
};

#endif