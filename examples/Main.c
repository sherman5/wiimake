#include <stdint.h>

#include "include/lib.h"

#include "TimesTwo.h"

int _main(void)
{
    *((uint32_t*) 0x80001800) = TimesTwo(8);
    *((float*) 0x80001804) = sin(30.0);
    *((float*) 0x80001808) = cos(-180.0);
}
