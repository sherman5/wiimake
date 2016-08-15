#include <stdint.h>

#include <TestLibrary1a.h>
//#include "TestLibrary1b.h"
#include <TestLibrary2.h>

int _main(void) {

    *((uint32_t*) 0x80001800) = timesTwo(0x04);
//    *((uint32_t*) 0x80001804) = timesThree(0x04);
    *((uint32_t*) 0x80001808) = timesFour(0x04);
    *((float*) 0x8000180C) = 1.5f;

}
