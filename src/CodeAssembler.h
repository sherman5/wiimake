#ifndef CODE_ASSEMBLER_H
#define CODE_ASSEMBLER_H

#include <string>
#include <vector>
#include <utility>

/*
First field (string) stores path to file.
Second field (uint32_t) stores length of file 
until FindCodeAllocation() is called. It then holds
    the address where to code is injected.
*/ 

typedef std::vector< std::pair<std::string, uint32_t> > FileList;

class CodeAssembler {

private:

    std::string m_dir;
    std::string m_region_file;  
    std::string m_linker_cmd;

    std::vector<std::string> m_c_files;
    FileList m_obj_files;
    std::vector< std::pair<uint32_t, uint32_t> > m_regions;
    uint32_t m_inject_addr;
    uint32_t m_stack_setup_addr;
    
    void GetSourceFiles();

    void CompileSourceFiles();
    void CreateDummyLinkerScript();
    void StoreRawCodeAsText();
    void GetObjectFileLengths();
    void FindCodeAllocation();    
    void CreateStackSetupFiles();
    void CreateRealLinkerScript();
    void Link();
    void StoreExecutableAsText();
    std::vector< std::pair<uint32_t, uint32_t> > GetCodeToInject();

public:

    CodeAssembler(std::string, std::string, std::vector<std::string>);

    uint32_t GetMainAddress();
    std::vector< std::pair<uint32_t, uint32_t> > GetRawASM();
    void CleanDirectory();
    
};

#endif
