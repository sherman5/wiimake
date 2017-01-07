#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H

#include "../LowLevel/LowLevel.h"
#include "../ArgumentParsing/Arguments.h"

/* big picture steps for compiling, allocating, linking code */
namespace Builder
{
    /* compile, allocate, link code */
    ASMcode getASM(Arguments&);

    /* compile files in directory, return list of all object files
       (including files from libraries) */
    FileList getObjectFiles(FileList, FileList = FileList(),
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
}

/* manage sections of code in object file */
namespace CodeSections
{
    /* get names of all sections on list of object files */
    void storeNames(SectionList&, FileList&);

    /* get sizes of each section */
    void storeSizes(SectionList&);
}

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
}

/* handle creation of linker scripts */
namespace LinkerScript
{
    /* create script to store sizes of sections */
    void CreateTempScript(SectionList&, std::string);

    /* create linker script to put sections in correct addresses */
    void CreateFinalScript(SectionList&, std::string);
}

#endif

