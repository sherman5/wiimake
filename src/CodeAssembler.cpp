#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "CodeAssembler.h"
#include "TextFileParser.h"

CodeAssembler::CodeAssembler(std::string path) {

    if (path.back() == '/') {path.pop_back();}
    m_dir_path = path;

}

void CodeAssembler::Compile() {
    
    StoreFileNames();

    std::string compile_cmd, obj_file;
    std::vector<std::string>::iterator it = m_file_names.begin();
    for (; it != m_file_names.end(); ++it) {

        obj_file = (*it);
        obj_file.back() = 'o';
        compile_cmd = "powerpc-eabi-gcc -c " + (*it) + " -o " + obj_file;

        FILE* compiler = popen(compile_cmd.c_str(), "r");

        char c;
        while ((c = std::getc(compiler)) != EOF) {
            std::cout << c;
        }
        std::cout << std::endl;

    }

    StoreFileLengths();

}

void CodeAssembler::StoreFileNames() {

    std::string cmd = "ls " + m_dir_path + "/*.c";
    FILE* file_list = popen(cmd.c_str(), "r");
    
    char file [500];    
    char *pos;

    while (std::fgets(file, 500, file_list)) {

        if ((pos = strchr(file, '\n')) != NULL) {
        
            *pos = '\0';

        }

        m_file_names.push_back(file);

    }
    
}

void CodeAssembler::StoreObjdump() {

    std::vector<std::string>::iterator it = m_file_names.begin();
    std::string obj_file, objdump_cmd;

    for (; it != m_file_names.end(); ++it) {

        obj_file = (*it);
        obj_file.back() = 'o';
        
        objdump_cmd = "powerpc-eabi-objdump -d " + obj_file;
        FILE* objdump = popen(objdump_cmd.c_str(), "r");

        obj_file.pop_back();
        std::ofstream outfile (obj_file + "txt");
        char c;
        while ((c = std::getc(objdump)) != EOF) {
            outfile << c;
        }
        outfile.close();

    }

}

void CodeAssembler::StoreFileLengths() {

    StoreObjdump();

    std::vector<std::string>::iterator it = m_file_names.begin();
    std::string dump_file;

    for (; it != m_file_names.end(); ++it) {

        dump_file = (*it);
        dump_file.pop_back();
        dump_file += "txt";
        TextFileParser parser (dump_file, objdumpFile);
        m_file_lengths.push_back(parser.GetNumberOfLines());

    }
    
}

