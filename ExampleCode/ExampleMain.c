#include <stdint.h>

#include <ExampleLibrary.h>

int _main(void) {

    *((uint32_t*) 0x80001800) = 0xABC;
    *((uint32_t*) 0x80001804) = timesTwo(0x04);
    *((float*) 0x80001808) = 1.5f;

}
