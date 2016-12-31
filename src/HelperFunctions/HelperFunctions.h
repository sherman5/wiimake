#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdint.h>
#include <string>
#include <utility>

#include "../MainProgram/Global.h"

/* big picture steps for compiling, allocating, linking code */
namespace Builder
{
    /* compile, allocate, link code */
    ASMcode getASM(Arguments&);

    /* compile files in directory, return list of all object files
       (including files from libraries) */
    FileList getObjectFiles(std::string, FileList = FileList(),
        FileList = FileList());

    /* calculate sizes of sections and find allocation in memory */
    SectionList getSectionAddresses(FileList&, Arguments&);

    /* add stack setup to call back after code is run */
    void addStackSetup(SectionList&, Arguments&);

    /* link code into final executable */
    ASMcode getLinkedCode(SectionList&);

    /* add original instruction, overwrite nop line in code */
    void addOriginalInstruction(ASMcode&, Arguments&);

    /* build library from source directory of c files */
    void buildLibrary(Arguments&);

    /* remove all temporary files created in the build process */
    void cleanDirectory();
};

/* manage sections of code in object file */
namespace CodeSections
{
    /* get names of all sections on list of object files */
    void storeNames(SectionList&, FileList&);

    /* get sizes of each section */
    void storeSizes(SectionList&);
};

/* manage memory regions in RAM */
namespace Memory
{
    /* find an allocation of code in available regions - can't change
       original memory regions object since the first address given
       in args.memRegions is used for stack setup */
    void findCodeAllocation(SectionList&, const Arguments&);

    /* store the address from given region for this section */
    void storeAddress(Section&, MemRegion&);

    /* verify allocation was done correctly */
    void checkAllocation(const SectionList&);
};

/* compile lists of C files */
namespace Compiler
{
    /* compile files in directory, using include paths given */
    FileList compile(std::string, FileList = FileList());
};

/* link lists of object files */
namespace Linker
{
    /* link all sections, using linker script provided */
    void link(SectionList&, std::string, std::string);
};    

/* handle creation of linker scripts */
namespace LinkerScript
{
    /* create script to store sizes of sections */
    void CreateTempScript(SectionList&, std::string);

    /* create linker script to put sections in correct addresses */
    void CreateFinalScript(SectionList&, std::string);
};

/* manipulate and parse object files */
namespace ObjectFile
{
    /* get all code from object file */
    ASMcode extractASM(std::string);

    /* get list of named sections in object file */
    std::vector<std::string> getNamedSections(std::string);

    /* name relevant sections in object file */
    void renameSections(std::string, std::string="");

    /* get single line of code from object file */
    std::pair<uint32_t, uint32_t> getLine(std::string, std::ifstream&);

    /* verify current line is beginning of line of code */
    bool lineOfCode(std::string);

};

/* parse symbol table */
namespace SymbolTable
{
    /* parse single line of table - record section size */
    void parseLine(std::ifstream&, std::string, std::vector<unsigned>&);
    
    /* gret sizes of all sections in this symbol table */
    std::vector<unsigned> getSizes(std::string, int);
};

/* handle all operations with .iso file */
namespace ISO
{
    /* get the DOL offset of a 32-bit RAM address */
    uint32_t DOLoffset(uint32_t, Table&);

    /* read 32-bit value from RAM address */
    uint32_t read(uint32_t, Arguments&);
    uint32_t read(std::string, Arguments&);

    /* write 32-bit value to RAM address */
    void write(uint32_t, uint32_t, Arguments&);

    /* save the current code from the regions provided */
    void saveState(Arguments&);

    /* load code from save file */
    void loadState(Arguments&);

    /* inject code into iso */
    void injectCode(ASMcode&, Arguments&);
};

/* run system specific commands */
namespace System
{
    /* command used for deletint files */
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

#endif
