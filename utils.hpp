#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <string>

bool get_next_request(std::ifstream& f, unsigned int* at, unsigned int* tr);

#endif