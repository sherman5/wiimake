#ifndef TESTING_HEADER_H
#define TESTING_HEADER_H

#include <iostream>
#include <string>

inline void displayHeader(std::string message)
{
    std::cout << "\033[33m\n============== " <<
    message
    << " ==============\n\033[39m" << std::endl;
}

#endif

