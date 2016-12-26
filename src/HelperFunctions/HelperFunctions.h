#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <string>
#include <stdint.h>

#include "Setup.h"

/* convenient typedef's */
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;
typedef std::vector<std::string> FileList;
typedef std::vector<Section> SectionList;

/* big picture steps for compiling, allocating, linking code */
namespace Builder
{
    ASMcode getASM(std::string, MemoryConfig&, FileList&, FileList&);

    FileList getObjectFiles(std::string, FileList = FileList(),
        FileList = FileList());

    SectionList getSectionAddresses(FileList&, MemoryConfig&);

    ASMcode getLinkedCode(SectionList&);

    void cleanDirectory();

};

namespace CodeSections
{
    void storeNames(SectionList&, FileList&);

    void storeSizes(SectionList&);

    void findCodeAllocation(SectionList&, MemoryConfig&);
};

namespace LinkerScript
{
    void CreateTempScript(SectionList&, std::string);

    void CreateFinalScript(SectionList&, std::string);
};

namespace Compiler
{
    FileList compile(std::string, FileList&);
};

namespace Linker
{
    void link(SectionList&, std::string, std::string);
};    

namespace System
{
    #ifdef GCI_WINDOWS
        const std::string rm = "del";
    #else
        const std::string rm = "rm";
    #endif

    /* return list of all files in a directory with given extension */
    FileList getFiles(std::string, std::string);

    /* runs a command and waits for it to finish */
    int runCMD(std::string);

};

namespace ObjectFile
{
    ASMcode extractASM(std::string);

    std::vector<std::string> getNamedSections(std::string);

    void renameSections(std::string, std::string);
};

namespace SymbolTable
{
    std::vector<unsigned> getSizes(std::string, int);
};

namespace ISO
{
    /* read 32-bit value from RAM address */
    uint32_t read(std::string, uint32_t);

    /* save the current code from the regions provided */
    void saveState(std::string, MemoryConfig&, std::string);

    /* load code from save file */
    void loadState(std::string, std::string);

    /* inject code into iso */
    void injectCode(std::string, ASMcode);
};

namespace Memory
{


};

#endif
