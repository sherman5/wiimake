#ifndef GCI_H
#define GCI_H

#include "../HelperFunctions/HelperFunctions.h"
#include "Global.h"

namespace GCI
{
    /* inject code into iso */
    void CreateISO(Arguments&);

    /* save iso state */    
    void SaveISO(Arguments&);

    /* load iso state from file */
    void LoadISO(Arguments&);
    
    /* read address from iso */    
    std::string ReadAddr(Arguments&);

    /* create static library from files in given directory */    
    void CreateLibrary(Arguments&);
}

#endif
