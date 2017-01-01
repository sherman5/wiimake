#ifndef ISO_CREATION_H
#define ISO_CREATION_H 

#include "../src/HelperFunctions/HelperFunctions.h"

inline void createTestIso()
{
    /* delete old iso */
    System::runCMD(System::rm + " ../tests/test.iso");
     
    /* create copy of reference */
    System::runCMD(System::cp + " ../tests/reference.iso ../tests/test.iso");
}

#endif