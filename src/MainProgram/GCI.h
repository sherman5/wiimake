#ifndef GCI_H
#define GCI_H

#include "../HelperFunctions/HelperFunctions.h"

namespace GCI
{
    /* inject code into iso */
    void CreateISO(std::string, std::string, MemoryConfig&, FileList&,
        FileList&, bool);

    /* save iso state, only save addresses in regions contained
       in mem_config */    
    void SaveISO(std::string, MemoryConfig&, std::string);

    /* load iso state from file */
    void LoadISO(std::string, std::string);
    
    /* read address from iso */    
    std::string ReadAddr(std::string, std::string);

    /* create static library from files in given directory */    
    void CreateLibrary(std::string, std::string);
}

#endif
