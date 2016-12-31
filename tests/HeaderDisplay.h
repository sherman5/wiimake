#ifndef TESTING_HEADER_H
#define TESTING_HEADER_H

#include <iostream>
#include <string>
#include <cmath>

inline void displayHeader(std::string message)
{
    int len = 60 - message.length() - 4;
    std::string pad (len / 2, '=');

    std::cout << "\033[33m\n" << std::setw(60 - len / 2) << std::left
        << pad + "  " + message << pad + "\n\033[39m" << std::endl;
}

#endif

