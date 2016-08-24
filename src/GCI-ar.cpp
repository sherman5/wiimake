#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>

#include "FileOperations.h"

typedef std::vector<std::string> FileList;

/* compile c files and return list of object files */
FileList GetObjects(std::string dir) {

    /* get a list of all c files */
    FileList sources = get_files(dir, "c");
    FileList objects;

    /* iterate through c files */
    FileList::iterator it = sources.begin();
    for (; it != sources.end(); ++it) {

        /* run compile command */
        std::string compile_cmd = "powerpc-eabi-gcc -c " + *it + " -o " + change_ext(*it, "o");
        FILE* compiler = popen(compile_cmd.c_str(), "r");

        /* output compiler output */
        char c;
        while ((c = std::getc(compiler)) != EOF) {
            std::cout << c;
        }
        std::cout << std::endl;

        /* close compile command, add object file */
        pclose(compiler);
        objects.push_back(change_ext(*it, "o"));

    }

    return objects;

}

/* create static library from files with given name */
void CreateLibrary(std::string name, FileList files) {
    
    /* add all files to archive command */
    std::string ar_cmd = "powerpc-eabi-ar -cvr " + name;
    for (auto it = files.begin(); it != files.end(); ++it) {

        ar_cmd += " " + *it;

    }

    /* run archive command to create static library */
    FILE* ar = popen(ar_cmd.c_str(), "r");
    pclose(ar);

}

int main(int argc, char* argv[]) {

    /* must provide 2 arguments */
    if (argc != 3) {

        std::cout << "ERROR: expected exactly 2 arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " LIB_NAME PATH_TO_SOURCE_FILES" << std::endl;

    } else {

        /* normalize source path and record argument values */
        std::string lib_name = argv[1];
        std::string source_path = argv[2];
        if (source_path.back() == '/') {source_path.pop_back();}

        /* get object files */
        std::vector<std::string> obj_files = GetObjects(source_path);

        /* rename sections in a unique way to differentiate between object files */
        for (unsigned int i = 0; i < obj_files.size(); ++i) {

            rename_sections(obj_files[i], std::to_string(i));

        }

        /* create the static library */
        CreateLibrary(lib_name, obj_files);

    }

}
