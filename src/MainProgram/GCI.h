#ifndef GCI_H
#define GCI_H

#include "../HelperFunctions/HelperFunctions.h"
#include "Global.h"

namespace GCI
{
    /* check that valid args are passed */
    void checkArgs(Arguments&);

    /* inject code into iso */
    void createISO(Arguments&);

    /* save iso state */    
    void saveISO(Arguments&);

    /* load iso state from file */
    void loadISO(Arguments&);
    
    /* read address from iso */    
    uint32_t readAddress(Arguments&);

    /* create static library from files in given directory */    
    void createLibrary(Arguments&);
};

#endif
