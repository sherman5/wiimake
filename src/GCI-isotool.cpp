#include <iostream>
#include <string>

#include "ISOhandler.h"

int main(int argc, char* argv[]) {

    /* must provide 2 arguments */
    if (argc != 3) {

        std::cout << "ERROR: expected exactly 2 arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " ISO_FILE READ_ADDRESS" << std::endl;

    } else {

        /* print 32-bit instruction at address READ_ADDRESS in file ISO_FILE */
        ISOhandler iso (argv[1]);
        std::cout << std::hex << iso.IsoRead(std::stoul(argv[2],nullptr,16)) << std::endl;

    }

}
