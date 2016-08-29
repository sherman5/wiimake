#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "CodeAssembler.h"
#include "RegionFileParser.h"
#include "ObjdumpFileParser.h"
#include "FileOperations.h"

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

/* constructor for CodeAssembler class */
CodeAssembler::CodeAssembler(std::string source_dir,
                             std::string region_file,
                             std::vector<std::string> include_dirs,
                             std::vector<std::string> libs) {

    /* get source files */
    if (source_dir.back() == '/' || source_dir.back() == '\\') {source_dir.pop_back();}
    m_c_files = get_files(source_dir, "c");

    /* store include directories */
    m_include_dirs = concat_vector(include_dirs, "-I");

    /* get available memory regions */
    RegionFileParser parser (region_file);
    for (RegionFileParser::iterator it = parser.begin(); !it.atEnd(); ++it) {

        m_regions.push_back(std::make_pair((*it).first, (*it).second));

    }

    /* get injection addresses for main and the stack setup */
    m_inject_addr = parser.GetInjectionPoint();
    m_stack_setup_addr = m_regions[0].first;       

    /* leave enough room for stack setup */
    m_regions[0].first += 0xC; 

    /* iterate through provided libraries */
    for (auto lib_it = libs.begin(); lib_it != libs.end(); ++lib_it) {

        /* add library to list of files to be linked */
        m_linked_files += *lib_it + " ";

        /* store each named section in library file */
        auto sections = get_named_sections(*lib_it);
        for (auto sect_it = sections.begin(); sect_it != sections.end(); ++sect_it) {

            m_sections.push_back(std::make_pair(*lib_it + " (" + *sect_it + ")", 0));

        }

    }

}

/* provides the logic for the main function of this class:
   call the steps to compile, allocate, and link the code
   in m_source_dir */
void CodeAssembler::GetRawASM() {

    CompileSourceFiles();
    CreateDummyLinkerScript();
    StoreRawCodeAsText();
    /*GetSectionLengths();
    FindCodeAllocation();    
    CreateBranchFiles(); 
    CreateRealLinkerScript();
    Link();
    return GetCodeToInject();*/

}

/* compile all .c files, store .o files */
void CodeAssembler::CompileSourceFiles() {
    
    /* iterate through all .c files */
    for (auto it = m_c_files.begin(); it != m_c_files.end(); ++it) {

        /* command to compile each .c file */
        std::string compile_cmd = "powerpc-eabi-gcc -c "+ m_include_dirs + *it + " -o " + change_ext(*it, "o");

        /* display and run compile command */
        std::cout << compile_cmd << std::endl;
        run_cmd(compile_cmd.c_str());

        /* add object file to list of files to be linked */
        m_linked_files += change_ext(*it, "o") + " ";
        
        /* rename relevent sections in object file */
        rename_sections(change_ext(*it, "o"));

        /* get sections from object file and store them */
        auto sections = get_named_sections(change_ext(*it, "o"));
        for (auto sect_it = sections.begin(); sect_it != sections.end(); ++sect_it) {

            m_sections.push_back(std::make_pair(change_ext(*it, "o") + " (" + *sect_it + ")", 0));

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
    
    /* iterate through all sections (reverse order to put libs last) */
    unsigned int label = 1;
    for (auto it = m_sections.rbegin(); it != m_sections.rend(); ++it) {

        /* write section header for each section */
        script << "gci_" << label << " :\n{\n\t" << (*it).first << "\n}\n";
        script << ". = ADDR(gci_" << label << ") + SIZEOF(gci_" << label << ");" << std::endl;
        label++;

    }

    /* write final line of linker script */
    script << "}" << std::endl;
    script.close();

}

/* run the linker with the dummy script and store the output */
void CodeAssembler::StoreRawCodeAsText() {

    /* link files with dummy script */
    std::string linker_cmd = "powerpc-eabi-ld -e _main --script linker_script.txt " + m_linked_files;
    std::cout << linker_cmd << std::endl;
    run_cmd(linker_cmd.c_str());
    
    /* store objdump in text file */    
    std::string cmd = "powerpc-eabi-objdump -D a.out > RawCode.txt";
    run_cmd(cmd.c_str());
    
}

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

}

/* find an allocation of sections given the available memory */
void CodeAssembler::FindCodeAllocation() {
    
    /* need working copy of sections (can't sort m_sections) */
    SectionList temp = m_sections;

    /* sort mem regions and code by size */
    std::sort (m_regions.begin(), m_regions.end(), region_comp);
    std::sort (temp.begin(), temp.end(), code_comp);

    /* store largest code section in region with most space */
    for (unsigned int i = 0; i < temp.size(); ++i) {

        /* ignore sections with no size */
        if (temp[i].second == 0) {break;}

        /* find correct section, set second field equal to inject address */
        auto section = temp[i];
        auto pos = std::find(m_sections.begin(), m_sections.end(), section);
        (*pos).second = m_regions[0].first;

        /* update region size, resort regions */
        m_regions[0].first += temp[i].second;
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
    run_cmd("powerpc-eabi-as stack_setup.s -o stack_setup.o");
    run_cmd("powerpc-eabi-as inject_point.s -o inject_point.o");

}

/* create the linker script using calculated inject addresses */
void CodeAssembler::CreateRealLinkerScript() {

    /* open up text file */
    std::ofstream script ("linker_script.txt");

    /* write first line of linker script */
    script << "SECTIONS {" << std::endl;
    
    /* iterate through all sections (reverse order to put libs last) */
    unsigned int label = 1;
    for (auto it = m_sections.rbegin(); it != m_sections.rend(); ++it) {

        /* write section header for each section */
        script << "gci_" << label << " 0x" << std::hex << (*it).second << " :\n{\n" << (*it).first << "\n}\n";
        label++;

    }

    /* write sections for branch files */
    script << "gci_" << label++ << " 0x" << std::hex << m_inject_addr << " :" << std::endl;
    script << "{\ninject_point.o\n}" << std::endl;
    
    script << "gci_" << label << " 0x" << std::hex << m_stack_setup_addr << " :" << std::endl;
    script << "{\nstack_setup.o\n}" << std::endl;

    /* write final line of linker script */
    script << "}" << std::endl;
    script.close();

}

/* link all files with actual linker script */
void CodeAssembler::Link() {

    /* add branch files to list of files to link */
    m_linked_files += " stack_setup.o inject_point.o";

    /* run linker */
    std::string linker_cmd = "powerpc-eabi-ld -e _main --script linker_script.txt " + m_linked_files;
    run_cmd(linker_cmd.c_str());
    
    /* store resulting executable as text file */
    run_cmd("powerpc-eabi-objdump -D a.out > exec.txt");

    /* remove a.out */

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
        ret_val.push_back(std::make_pair((*it).first, (*it).second));

    }

    return ret_val;

}

/* delete all temp files created in the process */
void CodeAssembler::CleanDirectory() {

    /* put all file names in vector */
    FILE* rm_call;
    std::vector<std::string> files = {
        "rm linker_script.txt",
        "rm RawCode.txt",
        "rm a.out",
        "rm exec.txt",
        "rm inject_point.s",
        "rm inject_point.o",
        "rm stack_setup.s",
        "rm stack_setup.o"
    };

    /* iterate through the vector and delete each one */
    std::vector<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it) {

        rm_call = popen((*it).c_str(), "r");
        pclose(rm_call);

    }
    
}


