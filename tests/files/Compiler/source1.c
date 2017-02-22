#include <stdint.h>

#include <lib1.h>
#include "source2.h"

int _main(void)
{
    *((uint32_t*) 0x80001800) = TimesTwo(0x08);
    *((float*) 0x80001808) = test_sin(30.0);
}

