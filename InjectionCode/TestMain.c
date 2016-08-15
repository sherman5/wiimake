#include <stdint.h>

#include <TestLibrary1a.h>
#include <TestLibrary1b.h>
#include <TestLibrary2.h>

int _main(void) {

    *((uint32_t*) 0x80001800) = 0xABC;
    *((uint32_t*) 0x80001804) = timesTwo(0x04);
    *((uint32_t*) 0x80001808) = timesThree(0x04);
    *((uint32_t*) 0x8000180C) = timesFour(0x04);
    *((float*) 0x80001810) = 1.5f;

}
