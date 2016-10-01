#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>

#include "FileOperations.h"


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


        /* create the static library */
        CreateLibrary(lib_name, obj_files);

    }

}
