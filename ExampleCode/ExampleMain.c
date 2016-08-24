#include <stdint.h>

#include <mml/Controller.h>
#include <mml/MeleeMath.h>

int _main(void) {

    *((uint32_t*) 0x80001800) = 0xABC;
    *((float*) 0x80001808) = mml_sin(30.0);
    button_addr = ((uint32_t*) 0x80003f00);
    Press('A');

}
