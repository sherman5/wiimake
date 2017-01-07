#include "LowLevel.h"

#include <iostream>

//TODO: throw exception with compiler error
FileList Compiler::compile(FileList& sources, FileList includePaths,
TokenList compileFlags)
{
    /* list of object files */
    FileList objects;

    /* string to hold include paths */
    std::string includes, flags;

    /* get include and flag strings */
    for (auto& path : includePaths)
    {
        includes += "-I" + path + " ";
    }

    for (auto& flag : compileFlags)
    {
        flags += flag + " ";
    }    

    /* iterate through c files */
    for (auto& file : sources)
    {
        /* create compile command */
        std::string cmd = "powerpc-eabi-gcc -c -fno-builtin " + flags +
            includes + file + " -o " + CHANGE_EXT(file, "o");
        
        /* add to object list */
        objects.push_back(CHANGE_EXT(file, "o"));
    
        /* display and run command */
        System::runCMD(cmd, true);
     }

    /* return list of objects */
    return objects;
}
