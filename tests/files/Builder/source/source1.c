#include <stdint.h>
#include <libmath.h>

#include "source2.h"

void _main(void)
{
    *((uint32_t*) 0x80001800) = TimesTwo(0x08);
    *((float*) 0x80001808) = sin(30.0);
}
