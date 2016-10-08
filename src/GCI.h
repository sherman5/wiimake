#ifndef GCI_H
#define GCI_H

#include "ISOhandler.h"
#include "MemoryConfig.h"
#include "CodeAssembler.h"

namespace GCI {

    /* inject code into iso */
    void CreateISO(ISOhandler&, MemoryConfig&, CodeAssembler&, bool=false);

    /* save iso state, only save addresses in regions contained in mem_config */    
    void SaveISO(ISOhandler&, MemoryConfig&, std::string);

    /* load iso state from file */
    void LoadISO(ISOhandler&, std::string);
    
    /* read address (32-bit unsigned int) from iso */    
    std::string ReadAddr(ISOhandler&, uint32_t);

    /* read address (string version of 32-bit unsigned int) from iso */    
    std::string ReadAddr(ISOhandler&, std::string);

    /* create static library from files in given directory */    
    void CreateLibrary(std::string, std::string);

}

#endif
