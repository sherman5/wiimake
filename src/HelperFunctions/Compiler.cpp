#include "HelperFunctions.h"

#include <iostream>

/* change extension of file name */
#define CHANGE_EXT(file, ext) (std::string(file).substr(0, std::string(file).find_last_of('.') + 1) + ext)

FileList Compiler::compile(std::string dir, FileList& includePaths)
{
    /* list of object files */
    FileList objects;

    /* get a list of all c files */
    auto sources = System::getFiles(dir, "c");

    /* delete any pre-existing object file in directory */
    System::runCMD(System::rm + " *.o");

    /* string to hold include paths */
    std::string includes;

    /* add all include paths */
    for (auto it = includePaths.begin(); it != includePaths.end(); ++it)
    {
        includes += "-I" + *it + " ";
    }    

    /* iterate through c files */
    for (auto it = sources.begin(); it != sources.end(); ++it) {

        /* create compile command */
        std::string cmd = "powerpc-eabi-gcc -c " + includes
            + *it + " -o " + CHANGE_EXT(*it, "o");
        
        /* add to object list */
        objects.push_back(CHANGE_EXT(*it, "o"));
    
        /* display and run command */
        std::cout << cmd << std::endl;
        System::runCMD(cmd);

        //TODO: display output of compiler

     }

    /* return list of objects */
    return objects;
}
