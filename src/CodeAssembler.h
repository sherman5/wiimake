#ifndef CODE_ASSEMBLER_H
#define CODE_ASSEMBLER_H

#include <string>
#include <vector>
#include <utility>

#include "MemoryConfig.h"

/*
string stores path to file (section)
uint32_t stores length of file until FindCodeAllocation() is
called. It then holds the address where to code is injected.
*/ 
typedef std::vector< std::pair<std::string, uint32_t> > SectionList;

/* convenient typedefs */
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;
typedef std::vector< std::pair<uint32_t, uint32_t> > MemoryRegions;

class CodeAssembler {

private:

    /* string containing all include directoires with -I flag */
    std::string m_include_dirs;

    /* string of all object files and static libraries */
    std::string m_linked_files;

    /* source directory */
    std::string m_source_dir;

    /* sections of the final binary */
    SectionList m_sections;   

    /* regions of memory available */
    MemoryRegions m_regions;

    /* address for the branch files */
    uint32_t m_inject_addr;
    uint32_t m_stack_setup_addr;

    /************** INIT FUNCTIONS **************************/

    /* store info from the memory config */
    void ProcessMemRegions(MemoryConfig);

    /* store libraries to link against */
    void ProcessLibs(std::vector<std::string>);
    
    /***** FUNCTIONS TO COMPILE, LINK, and ALLOCATE CODE ****/

    /* compile all .c files, store .o files */
    void StoreObjectFiles();

    /* create a linker script that includes all files, but not
       at the correct addresses */
    void CreateDummyLinkerScript();

    /* run the linker with the dummy script and store the output */
    void StoreRawCodeAsText();

    /* get the length of each section */
    void GetSectionLengths();

    /* find an allocation of sections given the available memory */
    void FindCodeAllocation();    

    /* create the object files which branch to main at the inject point */
    void CreateBranchFiles();

    /* create the linker script using calculated inject addresses */
    void CreateRealLinkerScript();

    /* link all files with actual linker script */
    void Link();

    /* return the lines of code at each address */
    ASMcode GetCodeToInject();


public:

    /* constructor */
    CodeAssembler(std::string,
                  MemoryConfig,
                  std::vector<std::string>,
                  std::vector<std::string>);

    /* provides the logic for the main function of this class:
       call the steps to compile, allocate, and link the code
       in m_source_dir */
    ASMcode GetRawASM();

    /* delete all temp files created in the process */
    void CleanDirectory();
    
};

#endif
