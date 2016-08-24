#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>

#include "FileOperations.h"

typedef std::vector<std::string> FileList;

FileList GetSources(std::string dir) {

    FileList sources;

    std::string cmd = "ls " + dir + "/*.c";
    FILE* file_list = popen(cmd.c_str(), "r");
    
    char file[500], *pos;    

    while (std::fgets(file, 500, file_list)) {

        if ((pos = strchr(file, '\n')) != NULL) {
        
            *pos = '\0';

        }

        sources.push_back(file);
        
    }

    pclose(file_list);
    return sources;

}

FileList GetObjects(std::string dir) {

    FileList sources = GetSources(dir);
    FileList objects;

    FileList::iterator it = sources.begin();
    for (; it != sources.end(); ++it) {

        std::string compile_cmd = "powerpc-eabi-gcc -c " + *it;
        compile_cmd += " -o " + (*it).substr(0, (*it).length() - 1) + "o";
        FILE* compiler = popen(compile_cmd.c_str(), "r");

        char c;
        while ((c = std::getc(compiler)) != EOF) {
            std::cout << c;
        }
        std::cout << std::endl;

        pclose(compiler);
        objects.push_back((*it).substr(0,(*it).length() - 1) + "o");

    }

    return objects;

}

void RenameSections(FileList objs) {

    for (unsigned int i = 0; i < objs.size(); ++i) {

        std::string cmd = "powerpc-eabi-objcopy";
        cmd += " --rename-section .text=section" + std::to_string(i) + "_start";
        cmd += " --rename-section .rodata=rodata" + std::to_string(i);
        cmd += " --rename-section .data=data" + std::to_string(i);
        cmd += " --rename-section .sdata=sdata" + std::to_string(i);
        cmd += " --rename-section .gnu.attributes=attr" + std::to_string(i);
        cmd += " " + objs[i];
        std::cout << cmd << std::endl;
        FILE* obj_cpy = popen(cmd.c_str(), "r");
        pclose(obj_cpy);

    }

}

void CreateLibrary(std::string name, FileList files) {
    
    std::string ar_cmd = "powerpc-eabi-ar -cvr " + name;
    for (unsigned int i = 0; i < files.size(); ++i) {

        ar_cmd += " " + files[i];

    }

    FILE* ar = popen(ar_cmd.c_str(), "r");
    pclose(ar);

}

int main(int argc, char* argv[]) {

    if (argc != 3) {

        std::cout << "ERROR: expected exactly 2 arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " LIB_NAME PATH_TO_SOURCE_FILES" << std::endl;

    } else {

        std::string lib_name = argv[1];
        std::string source_path = argv[2];
        if (source_path.back() == '/') {source_path.pop_back();}

        std::vector<std::string> obj_files = GetObjects(source_path);

        RenameSections(obj_files);

        CreateLibrary(lib_name, obj_files);

    }

}
