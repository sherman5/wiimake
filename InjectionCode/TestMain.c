#include <stdint.h>

#include "TestLibrary.h"

unsigned int timesThree(unsigned int num) {

    return num * 3;

}

int _main(void) {

    *((uint32_t*) 0x80001800) = 0x04;
    *((uint32_t*) 0x80001804) = timesTwo(0x04);
    *((uint32_t*) 0x80001808) = timesThree(0x04);

}
