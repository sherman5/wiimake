#include <stdint.h>

#include <mml/MeleeMath.h>

#include "ExampleFunction.h"

int _main(void)
{
    *((uint32_t*) 0x80001800) = TimesTwo(0x08);
    *((float*) 0x80001808) = mml_sin(30.0);
}
