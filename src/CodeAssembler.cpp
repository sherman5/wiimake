#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "CodeAssembler.h"
#include "TextFileParser.h"

/****************** HELPER FUNCTIONS ********************/

bool region_comp(std::pair<uint32_t, uint32_t> a, std::pair<uint32_t, uint32_t> b) {

    return a.second - a.first > b.second - b.first;

}

bool code_comp(std::pair<std::string, uint32_t> a, std::pair<std::string, uint32_t> b) {

    return a.second > b.second;

}

std::string change_ext(std::string file, std::string ext) {

    std::string temp = file;
    temp.pop_back();
    return temp += ext;

}

/********************************************************/

CodeAssembler::CodeAssembler(std::string path, std::string regions, std::vector<std::string> libs) {

    if (path.back() == '/') {path.pop_back();}
    m_dir = path;
    m_region_file = regions;
    
    GetSourceFiles();
    
    std::vector<std::string>::iterator it = libs.begin();
    for (; it != libs.end(); ++it) {

        m_obj_files.push_back(std::make_pair((*it), 0));

    }

}

std::vector< std::pair<uint32_t, uint32_t> > CodeAssembler::GetRawASM() {

    CompileSourceFiles();
    CreateDummyLinkerScript();
    StoreRawCodeAsText();
    GetObjectFileLengths();
    FindCodeAllocation();    
    CreateRealLinkerScript();
    Link();
    StoreExecutableAsText();
/*
    popen("rm linker_script.txt", "r");
    popen("rm RawCode.txt", "r");
    popen("rm a.out", "r");
    popen("rm exec.txt", "r");
    std::string cmd = "rm " + m_dir + "/*.o";
    popen(cmd.c_str(), "r");
*/

    return GetCodeToInject();    

}

void CodeAssembler::GetSourceFiles() {

    std::string cmd = "ls " + m_dir + "/*.c";
    FILE* file_list = popen(cmd.c_str(), "r");
    
    char file[500], *pos;    

    while (std::fgets(file, 500, file_list)) {

        if ((pos = strchr(file, '\n')) != NULL) {
        
            *pos = '\0';

        }

        m_c_files.push_back(file);
        m_obj_files.push_back(std::make_pair(change_ext(file, "o"), 0));

    }
    
}

void CodeAssembler::CompileSourceFiles() {
    
    std::string compile_cmd;

    std::vector<std::string>::iterator it = m_c_files.begin();
    for (; it != m_c_files.end(); ++it) {

        compile_cmd = "powerpc-eabi-gcc -c -I" + m_dir + "/libs " + *it + " -o " + change_ext(*it, "o");

        FILE* compiler = popen(compile_cmd.c_str(), "r");

        char c;
        while ((c = std::getc(compiler)) != EOF) {
            std::cout << c;
        }
        std::cout << std::endl;

    }

}

void CodeAssembler::CreateDummyLinkerScript() {

    std::ofstream script ("linker_script.txt");

    script << "SECTIONS {" << std::endl;

    for (unsigned int i = 0; i < m_obj_files.size(); ++i) {

        script << "section" << i + 1 << " :" << std::endl;
        script << "{" << std::endl;
        script << m_obj_files[i].first << " (.text)" << std::endl;
        script << m_obj_files[i].first << " (.rodata)" << std::endl;
        script << "}" << std::endl;
        
    }

    script << "}" << std::endl;

    script.close();

}

void CodeAssembler::StoreRawCodeAsText() {

    std::string objs;
    FileList::iterator it = m_obj_files.begin();    
    for (; it != m_obj_files.end(); ++it) {
    
        objs += (*it).first + " ";

    }

    m_linker_cmd = "powerpc-eabi-ld -e _main --script linker_script.txt " + objs;
    FILE* linker = popen(m_linker_cmd.c_str(), "r");
    while (std::getc(linker) != EOF) {}

    std::string cmd = "powerpc-eabi-objdump -d a.out";
    FILE* objdump = popen(cmd.c_str(), "r");

    std::ofstream outfile ("RawCode.txt");
    char c;
    while ((c = std::getc(objdump)) != EOF) {
        outfile << c;
    }
    outfile.close();

}

void CodeAssembler::GetObjectFileLengths() {

    TextFileParser parser("RawCode.txt", objdumpFile);

    TextFileParser::iterator it = parser.begin();   
    unsigned int length = 0, index = 0;
    bool skipped_first_line = false;
    for (; !it.atEnd(); ++it) {

        if (it.newSection() && skipped_first_line) {

            m_obj_files[index++].second = length + 4; 
            length = 0;

        } else {

            length += 4;
            if (!skipped_first_line) { skipped_first_line = true; length -= 4;}

        }
        
    }

    m_obj_files[index].second = length + 4;

}

void CodeAssembler::FindCodeAllocation() {

    std::vector< std::pair<uint32_t, uint32_t> > regions;
    TextFileParser parser (m_region_file, regionFile);

    TextFileParser::iterator it = parser.begin();
    for (; !it.atEnd(); ++it) {

        regions.push_back(std::make_pair((*it).first, (*it).second));

    }
    
    std::sort (regions.begin(), regions.end(), region_comp);
    std::sort (m_obj_files.begin(), m_obj_files.end(), code_comp);

    for (unsigned int i = 0; i < m_obj_files.size(); ++i) {

        int length = m_obj_files[i].second;
        m_obj_files[i].second = regions[0].first;
        regions[0].first += length;
        std::sort (regions.begin(), regions.end(), region_comp);

    }
    
}

void CodeAssembler::CreateRealLinkerScript() {

    std::ofstream script ("linker_script.txt");

    script << "SECTIONS {" << std::endl;

    for (unsigned int i = 0; i < m_obj_files.size(); ++i) {

        script << "section" << i + 1 << " 0x" << std::hex << m_obj_files[i].second << " :" << std::endl;
        script << "{" << std::endl;
        script << m_obj_files[i].first << " (.text)" << std::endl;
        script << m_obj_files[i].first << " (.rodata)" << std::endl;
        script << "}" << std::endl;
        
    }

    script << "}" << std::endl;

    script.close();

}

void CodeAssembler::Link() {

    FILE* linker = popen(m_linker_cmd.c_str(), "r");
    
}

void CodeAssembler::StoreExecutableAsText() {

    FILE* exec_out = popen("powerpc-eabi-objdump -d a.out", "r");

    std::ofstream outfile ("exec.txt");
    char c;
    while ((c = std::getc(exec_out)) != EOF) {
        outfile << c;
    }
    outfile.close();

}

std::vector< std::pair<uint32_t, uint32_t> > CodeAssembler::GetCodeToInject() {

    std::vector< std::pair<uint32_t, uint32_t> > ret_val;

    TextFileParser parser ("exec.txt", objdumpFile);
    TextFileParser::iterator it = parser.begin();

    for (; !it.atEnd(); ++it) {

        ret_val.push_back(std::make_pair((*it).first, (*it).second));

    }


    return ret_val;

}
