#ifndef TDData
#define TDData
#include <iostream>

struct Command {
    std::string CommandName;
    void (*Call)(std::string);
};

#endif