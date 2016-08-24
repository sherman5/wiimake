#ifndef CODE_ASSEMBLER_H
#define CODE_ASSEMBLER_H

#include <string>
#include <vector>
#include <utility>

/*
string stores path to file (section).
uint32_t stores length of file 
until FindCodeAllocation() is called. It then holds
the address where to code is injected.
*/ 
typedef std::vector< std::pair<std::string, uint32_t> > SectionList;

typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;
typedef std::vector< std::pair<uint32_t, uint32_t> > MemoryRegions;

class CodeAssembler {

private:

    std::string m_include_dirs;
    std::string m_linked_files;

    std::vector<std::string> m_c_files;
    SectionList m_sections;   
    MemoryRegions m_regions;

    uint32_t m_inject_addr;
    uint32_t m_stack_setup_addr;
    
    void GetSourceFiles();

    void CompileSourceFiles();
    void CreateDummyLinkerScript();
    void StoreRawCodeAsText();
    void GetSectionLengths();
    void FindCodeAllocation();    
    void CreateBranchFiles();
    void CreateRealLinkerScript();
    void Link();
    ASMcode GetCodeToInject();

public:

    CodeAssembler(std::string,
                  std::string,
                  std::vector<std::string>,
                  std::vector<std::string>);

    ASMcode GetRawASM();
    void CleanDirectory();
    
};

#endif
