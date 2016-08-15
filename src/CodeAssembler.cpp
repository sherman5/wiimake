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
    
    GetSourceFiles();
    
    std::vector<std::string>::iterator it = libs.begin();
    for (; it != libs.end(); ++it) {

        m_obj_files.push_back(std::make_pair((*it), 0));

    }

    TextFileParser parser (regions, regionFile);

    TextFileParser::iterator iter = parser.begin();
    for (; !iter.atEnd(); ++iter) {

        m_regions.push_back(std::make_pair((*iter).first, (*iter).second));

    }
 
    m_stack_setup_addr = m_regions[0].first;       
    m_regions[0].first += 0xC; //leave room for call stack    

    m_inject_addr = parser.GetInjectionPoint();

}

std::vector< std::pair<uint32_t, uint32_t> > CodeAssembler::GetRawASM() {

    CompileSourceFiles();
    CreateDummyLinkerScript();
    StoreRawCodeAsText();
    GetObjectFileLengths();
    FindCodeAllocation();    
    CreateStackSetupFiles(); 
    CreateRealLinkerScript();
    Link();
    StoreExecutableAsText();
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
    pclose(file_list);
    
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

        pclose(compiler);

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
    pclose(linker);
    
    std::string cmd = "powerpc-eabi-objdump -d a.out";
    FILE* objdump = popen(cmd.c_str(), "r");

    std::ofstream outfile ("RawCode.txt");
    char c;
    while ((c = std::getc(objdump)) != EOF) {
        outfile << c;
    }
    outfile.close();
    pclose(objdump);

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
    
    std::sort (m_regions.begin(), m_regions.end(), region_comp);
    std::sort (m_obj_files.begin(), m_obj_files.end(), code_comp);

    for (unsigned int i = 0; i < m_obj_files.size(); ++i) {

        int length = m_obj_files[i].second;
        m_obj_files[i].second = m_regions[0].first;
        m_regions[0].first += length;
        std::sort (m_regions.begin(), m_regions.end(), region_comp);

    }
    
}

void CodeAssembler::CreateStackSetupFiles() {

    std::ofstream stack_setup ("stack_setup.s");

    stack_setup << ".global stack_setup" << std::endl;
    stack_setup << "stack_setup:" << std::endl;
    stack_setup << "bl _main" << std::endl;
    stack_setup << "mr 3, 23" << std::endl; //TODO: hardcoded instruction at inject point
    stack_setup << "b inject_point + 0x04" << std::endl;

    stack_setup.close();

    std::ofstream inject_point ("inject_point.s");

    inject_point << ".global inject_point" << std::endl;
    inject_point << "inject_point:" << std::endl;
    inject_point << "bl stack_setup" << std::endl;

    inject_point.close();

    FILE* assmbl = popen("powerpc-eabi-as stack_setup.s -o stack_setup.o", "r");
    pclose(assmbl);
    assmbl = popen("powerpc-eabi-as inject_point.s -o inject_point.o", "r");
    pclose(assmbl);

}

void CodeAssembler::CreateRealLinkerScript() {

    std::ofstream script ("linker_script.txt");

    script << "SECTIONS {" << std::endl;

    unsigned int i = 0;
    for (; i < m_obj_files.size(); ++i) {

        script << "section" << i + 1 << " 0x" << std::hex << m_obj_files[i].second << " :" << std::endl;
        script << "{\n" << m_obj_files[i].first << " (.text)" << std::endl;
        script << m_obj_files[i].first << " (.rodata)\n}" << std::endl;
        
    }

    script << "section" << i++ + 1 << " 0x" << std::hex << m_inject_addr << " :" << std::endl;
    script << "{\ninject_point.o\n}" << std::endl;
    
    script << "section" << i++ + 1 << " 0x" << std::hex << m_stack_setup_addr << " :" << std::endl;
    script << "{\nstack_setup.o\n}" << std::endl;

    script << "}" << std::endl;

    script.close();

}

void CodeAssembler::Link() {

    m_linker_cmd += "stack_setup.o inject_point.o";
    FILE* linker = popen(m_linker_cmd.c_str(), "r");
    pclose(linker);
    
}

void CodeAssembler::StoreExecutableAsText() {

    FILE* exec_out = popen("powerpc-eabi-objdump -d a.out", "r");

    std::ofstream outfile ("exec.txt");
    char c;
    while ((c = std::getc(exec_out)) != EOF) {
        outfile << c;
    }
    outfile.close();
    
    pclose(exec_out);

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

uint32_t CodeAssembler::GetMainAddress() {

    TextFileParser parser ("exec.txt", objdumpFile);

    return parser.GetMainInjectAddress();

}

void CodeAssembler::CleanDirectory() {

    FILE* rm_call;
    std::vector<std::string> files = {
        "rm linker_script.txt",
        "rm RawCode.txt",
        "rm a.out",
        "rm exec.txt",
        "rm inject_point.s",
        "rm inject_point.o",
        "rm stack_setup.s",
        "rm stack_setup.o",
        "rm " + m_dir + "/*.o"
    };

    std::vector<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it) {

        rm_call = popen((*it).c_str(), "r");
        pclose(rm_call);

    }
    
}


