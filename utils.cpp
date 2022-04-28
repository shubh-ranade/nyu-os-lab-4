#include "utils.hpp"
#include <sstream>

using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

// Utility functions for trimming string
string trim_leading(const string& s) {
    size_t first_idx_not_whitespace = s.find_first_not_of(WHITESPACE);
    return first_idx_not_whitespace == string::npos ? "" : s.substr(first_idx_not_whitespace);
}

string trim_trailing(const string& s) {
    size_t last_idx_not_whitespace = s.find_last_not_of(WHITESPACE);
    return last_idx_not_whitespace == string::npos ? "" : s.substr(0, last_idx_not_whitespace + 1);
}

string trim(const string& s) {
    return trim_trailing(trim_leading(s));
}

string getline_clean(ifstream& f) {
    string buffer;
    getline(f, buffer);
    buffer = trim(buffer);
    
    // empty line
    if(buffer == "" && !f.eof())
        return getline_clean(f);

    // line starts with #
    if(buffer[0] == '#' && !f.eof())
        return getline_clean(f);

    // assume well formed line otherwise
    return buffer;
}

bool get_next_request(ifstream& f, unsigned int* at, unsigned int* tr) {
    string buffer = getline_clean(f);
    if(buffer == "")
        return false;

    istringstream(buffer) >> *at >> *tr;
    return true;
}