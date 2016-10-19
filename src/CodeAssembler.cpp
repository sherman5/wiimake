#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "CodeAssembler.h"
#include "MemoryConfig.h"
#include "ObjdumpFileParser.h"
#include "SystemOperations.h"

/****************** HELPER FUNCTIONS ********************/

/* compare two regions of memory, return true if region a is larger: used for sorting */
bool region_comp(std::pair<uint32_t, uint32_t> a, std::pair<uint32_t, uint32_t> b) {

    return a.second - a.first > b.second - b.first;

}

/* compare two object files, return true if a is longer: used for sorting */
bool code_comp(std::pair<std::string, uint32_t> a, std::pair<std::string, uint32_t> b) {

    return a.second > b.second;

}

/********************************************************/

/********* CONSTRUCTOR AND INIT FUNCTIONS ***************/

/* constructor for CodeAssembler class */
CodeAssembler::CodeAssembler(std::string source_dir,
                             MemoryConfig mem_config,
                             std::vector<std::string> include_dirs,
                             std::vector<std::string> libs) {


    /* remove trailing backslash if present */
    if (source_dir.back() == '/' || source_dir.back() == '\\') {
    
        source_dir.pop_back();
    
    }

    /* store source directory */
    m_source_dir = source_dir;

    /* store include directories */
    m_include_dirs = SysOp::concatVector(include_dirs, "-I");

    /* process and store info from mem_config and libs */
    ProcessMemRegions(mem_config);
    ProcessLibs(libs);

}

/* store info from the memory config */
void CodeAssembler::ProcessMemRegions(MemoryConfig config) {

    /* iterate through available memory regions */
    auto it = config.begin();
    for (; it != config.end(); ++it) {

        /* store each region */
        m_regions.push_back(std::make_pair((*it).first, (*it).second));

    }

    /* get injection addresses for main and the stack setup */
    m_inject_addr = config.GetInjectAddress();
    m_stack_setup_addr = m_regions[0].first;       

    /* leave enough room for stack setup */
    m_regions[0].first += 0xC; 

}

/* store libraries to link against */
void CodeAssembler::ProcessLibs(std::vector<std::string> libs) {

    /* store libs as string to link against */
    m_linked_files = SysOp::concatVector(libs);
    
    /* iterate through libs */
    for (auto lib_it = libs.begin(); lib_it != libs.end(); ++lib_it) {

        /* get a vector of all named sections (no '.') in lib file */
        auto sections = SysOp::getNamedSections(*lib_it);

        /* iterate through named sections */
        auto section_it = sections.begin();
        for (; section_it != sections.end(); ++section_it) {

            /* store section */
            m_sections.push_back(
                std::make_pair(*lib_it + " (" + *sect_it + ")", 0));

        }

    }

}

/********************************************************/

/**************** PUBLIC FUNCTIONS **********************/

/* provides the logic for the main function of this class:
   call the steps to compile, allocate, and link the code
   in m_source_dir */
ASMcode CodeAssembler::GetRawASM() {

    StoreObjectFiles();
    CreateDummyLinkerScript();
    StoreRawCodeAsText();
    GetSectionLengths();
    FindCodeAllocation();    
    CreateBranchFiles(); 
    CreateRealLinkerScript();
    Link();
    return GetCodeToInject();

}


/* delete all temp files created in the process */
void CodeAssembler::CleanDirectory() {

    /* put all file names in vector */
    std::vector<std::string> files = {
        rm_cmd + " linker_script.txt",
        rm_cmd + " RawCode.txt",
        rm_cmd + " exec.txt",
        rm_cmd + " inject_point.s",
        rm_cmd + " inject_point.o",
        rm_cmd + " stack_setup.s",
        rm_cmd + " stack_setup.o"
    };

    /* iterate through the vector and run each cmd */
    std::vector<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it) {

        SysOp::runCMD(*it);

    }
    
}

/********************************************************/

/***** FUNCTIONS TO COMPILE, LINK, and ALLOCATE CODE ****/

/* compile all .c files, store .o files */
void CodeAssembler::StoreObjectFiles() {

    /* compile source files */
    SysOp::compileFiles(m_source_dir, m_include_dirs);

    /* get object files */
    auto objects = SysOp::getFiles(m_source_dir, "o");

    /* add object files to list of files to link */
    m_linked_files += " " + SysOp::concatVector(objects);
    
    /* iterate through each object file */    
    for (auto it = objects.begin(); it != objects.end(); ++it) {

        /* rename relevent sections */
        SysOp::renameSections(*it);

        /* get sections from object file and store them */
        auto sections = SysOp::getNamedSections(*it);

        /* iterate through named section in object file */
        auto section_it = sections.begin();
        for (; section_it != sections.end(); ++section_it) {

            m_sections.push_back(
                std::make_pair(*it + " (" + *sect_it + ")", 0));

        }

    }

}

/* create a linker script that includes all files, but not
   at the correct addresses */
void CodeAssembler::CreateDummyLinkerScript() {

    /* open up text file */
    std::ofstream script ("linker_script.txt");

    /* write first line of linker script */
    script << "SECTIONS {" << std::endl;

    /* section label */
    unsigned int label = 1;
    
    /* iterate through all sections (reverse order to put libs last) */
    for (auto it = m_sections.rbegin(); it != m_sections.rend(); ++it) {

        /* write section header */
        script << "gci_" << label << " :\n{\n\t" << (*it).first << "\n}\n";

        /* increment current address */
        script << ". = ADDR(gci_" << label << ") + SIZEOF(gci_"
               << label << ");" << std::endl;

        /* increment label */
        label++;

    }

    /* write final line of linker script */
    script << "}" << std::endl;

    /* close linker script */
    script.close();

}

/* run the linker with the dummy script and store the output */
void CodeAssembler::StoreRawCodeAsText() {

    /* create linker command */
    std::string cmd = "powerpc-eabi-ld -e _main --script linker_script.txt "
                        + m_linked_files;

    /* display linker command */
    std::cout << cmd << std::endl;

    /* run linker */
    SysOp::runCMD(cmd);
    
    /* store objdump in text file */    
    SysOp::runCMD("powerpc-eabi-objdump -D a.out > RawCode.txt");
    
    /* remove a.out */
    SysOp::runCMD(rm_cmd + " a.out");
    
}

//TODO: make this function cleaner/break up into more functions
/* get the length of each section */
void CodeAssembler::GetSectionLengths() {

    /* keep track of previous and current address to determine size */
    int prev_addr = -4, addr = 0, sect_ndx = m_sections.size() - 1; 

    /* iterate through linked executable file */
    ObjdumpFileParser parser("RawCode.txt");
    ObjdumpFileParser::iterator it = parser.begin();   

    do {
        
        ++it;

        /* new section is found */
        if (it.getSection() != "gci_" + std::to_string(m_sections.size() - sect_ndx)) {

            /* record size (plus buffer), reset reference address */
            m_sections[sect_ndx].second = addr - prev_addr + 0x04;
            prev_addr = addr;

            /* get the index of the next section if its not the end */
            if (it.getSection().find("gci") != std::string::npos) {

                std::string sec = it.getSection();
                sect_ndx = m_sections.size() - stoi(sec.substr(sec.find("_") + 1, sec.length()));

            }
            
        }

        /* update current address */
        addr = (*it).first;

    /* end loop once named sections run out */
    } while (it.getSection().find("gci") != std::string::npos);

    /* close file parser */
    it.close();

}

/* find an allocation of sections given the available memory */
void CodeAssembler::FindCodeAllocation() {
    
    /* need working copy of sections (can't sort m_sections itself) */
    SectionList temp = m_sections;

    /* sort mem regions and code by size */
    std::sort (m_regions.begin(), m_regions.end(), region_comp);
    std::sort (temp.begin(), temp.end(), code_comp);

    /* store largest code section in region with most space */
    for (unsigned int i = 0; i < temp.size(); ++i) {

        /* ignore sections with no size */
        if (temp[i].second == 0) {break;}

        /* find current section in m_sections */
        auto pos = std::find(m_sections.begin(), m_sections.end(), temp[i]);

        /* set second field equal to inject address */
        (*pos).second = m_regions[0].first;

        /* update region size */
        m_regions[0].first += temp[i].second;

        /* resort regions */     
        std::sort (m_regions.begin(), m_regions.end(), region_comp);

    }
    
}

/* create the object files which branch to main at the inject point */
void CodeAssembler::CreateBranchFiles() {

    /* This file sets up the call to main */
    std::ofstream stack_setup ("stack_setup.s");
    stack_setup << ".global stack_setup\nstack_setup:\nbl _main\nnop\nb inject_point + 0x04\n";
    stack_setup.close();

    /* This file puts a branch at the injection point */
    std::ofstream inject_point ("inject_point.s");
    inject_point << ".global inject_point\ninject_point:\nbl stack_setup\n";
    inject_point.close();

    /* assemble both files */
    SysOp::runCMD("powerpc-eabi-as stack_setup.s -o stack_setup.o");
    SysOp::runCMD("powerpc-eabi-as inject_point.s -o inject_point.o");

}

/* create the linker script using calculated inject addresses */
void CodeAssembler::CreateRealLinkerScript() {

    /* open up text file */
    std::ofstream script ("linker_script.txt");

    /* write first line of linker script */
    script << "SECTIONS {" << std::endl;
    
    /* section label */
    unsigned int label = 1;

    /* iterate through all sections (reverse order to put libs last) */
    for (auto it = m_sections.rbegin(); it != m_sections.rend(); ++it) {

        /* write section header for each section */
        script << "gci_" << label << " 0x" << std::hex << (*it).second
            << " :\n{\n" << (*it).first << "\n}\n";

        /* increment label */
        label++;

    }

    /* write section for inject_point */
    script << "gci_" << label++ << " 0x" << std::hex << m_inject_addr
           << " :" << std::endl;
    script << "{\ninject_point.o\n}" << std::endl;
    
    /* write section for stack_setup */
    script << "gci_" << label << " 0x" << std::hex << m_stack_setup_addr << " :" << std::endl;
    script << "{\nstack_setup.o\n}" << std::endl;

    /* write final line of linker script */
    script << "}" << std::endl;
    
    /* close linker script */
    script.close();

}

/* link all files with actual linker script */
void CodeAssembler::Link() {

    /* add branch files to list of files to link */
    m_linked_files += " stack_setup.o inject_point.o";

    /* run linker */
    SysOp::runCMD("powerpc-eabi-ld -e _main --script linker_script.txt "
                   + m_linked_files;);
    
    /* store resulting executable as text file */
    run_cmd("powerpc-eabi-objdump -D a.out > exec.txt");

    /* remove a.out */
    SysOp::runCMD(rm_cmd + " a.out");

}

/* return the lines of code at each address */
ASMcode CodeAssembler::GetCodeToInject() {

    /* vector of code to return */
    ASMcode ret_val;

    /* iterate through text dump of executable file */
    ObjdumpFileParser parser ("exec.txt");
    ObjdumpFileParser::iterator it = parser.begin();

    for (; !it.atEnd(); ++it) {

        /* stop when section name contains '.' */
        if (it.getSection().find(".") != std::string::npos) {break;}

        /* add line of code to vector */
        ret_val.push_back(std::make_pair((*it).first, (*it).second));

    }

    /* close file parser */
    it.close();

    /* return vector of code */    
    return ret_val;

}


/********************************************************/
