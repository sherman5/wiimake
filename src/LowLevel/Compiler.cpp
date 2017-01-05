#include "LowLevel.h"

#include <iostream>

//TODO: throw exception with compiler error
FileList Compiler::compile(FileList& sources, FileList includePaths)
{
    /* list of object files */
    FileList objects;

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
        std::string cmd = "powerpc-eabi-gcc -c -fno-builtin " + includes
            + *it + " -o " + CHANGE_EXT(*it, "o");
        
        /* add to object list */
        objects.push_back(CHANGE_EXT(*it, "o"));
    
        /* display and run command */
        System::runCMD(cmd, true);
     }

    /* return list of objects */
    return objects;
}
