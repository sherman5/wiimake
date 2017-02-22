#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H

#include "../ArgumentParsing/Arguments.h"
#include "../Global.h"

/* section in object file */
struct Section
{
    std::string path = ""; // path/to/objectfile (section name)
    unsigned int size = 0; // size of section (size of asm code)
    uint32_t address = 0; // address this section is injected to

    /* constructors */
    Section(std::string p) : path (p) {}
    Section(std::string p, uint32_t a) : path(p), address(a) {}

    /* used for sorting */
    bool operator<(const Section& other) { return size < other.size;}
    bool operator==(const Section& other) { return size == other.size;}
};

typedef std::vector<Section> SectionList;

/* big picture steps for compiling, allocating, linking code */
namespace Builder
{
    /* compile, allocate, link code */
    ASMcode getASM(Arguments&, unsigned&);

    /* add stack setup to call back after code is run */
    void addStackSetup(SectionList&, Arguments&);

    /* add original instruction, overwrite nop line in code */
    void addOverwrittenASM(ASMcode&, Arguments&, SectionList&);

    /* get regions of memory to zero out */
    ASMcode getZeroedMemory(Arguments&);

    /* remove all temporary files created in the build process */
    void cleanDirectory(bool);
}

/* manage sections of code in object file */
namespace CodeSections
{
    /* get names of all sections on list of object files */
    void storeNames(SectionList&, FileList&);

    /* get sizes of each section, return total size */
    unsigned storeSizes(SectionList&, Arguments&);
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
}

/* handle creation of linker scripts */
namespace LinkerScript
{
    /* create script to store sizes of sections */
    void CreateSizeScript(SectionList&, std::string);

    /* create linker script to put sections in correct addresses */
    void CreateFinalScript(SectionList&, std::string);
}

#endif

