#include <iostream>

#include "TextFileParser.h"
#include "ISOhandler.h"

int main(int argc, char* argv[]) {

    if (argc < 3) {

        std::cout << "too few arguments" << std::endl;

    } else {

        ISOhandler iso(argv[1]);

    }

}


