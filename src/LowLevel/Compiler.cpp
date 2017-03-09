#include "LowLevel.h"
#include "../Global.h"

const std::string default_flags =
    " -ffunction-sections -fdata-sections ";

std::string Compiler::compile(std::string file, TokenList compileFlags,
FileList includePaths)
{
    /* string to hold include paths */
    std::string includes, flags;

    /* get include and flag strings */
    for (auto& path : includePaths) { includes += "-I" + path + " ";}
    for (auto& flag : compileFlags) { flags += flag + " ";}    

    /* create compile command */
    std::string cmd = "powerpc-eabi-gcc -c " + default_flags + flags +
        includes + file + " -o " + CHANGE_EXT(file, "o");

    /* display and run command, throw on error */
    RUNTIME_ERROR(System::runCMD(cmd, true) != 0, "Compiler Error");

    /* return name of object file */
    return CHANGE_EXT(file, "o");
}

FileList Compiler::compileAll(FileList& sources, TokenList compileFlags,
FileList includePaths)
{
    /* list of object files */
    FileList objects;

    /* iterate through c files */
    for (auto& file : sources)
    {
        objects.push_back(Compiler::compile(file, compileFlags,
            includePaths));
    }

    /* return list of objects */
    return objects;
}
