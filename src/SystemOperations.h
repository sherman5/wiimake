#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(GCI_WINDOWS)

    #define GCI_WINDOWS
    
#endif

#include <string>
#include <vector>

#ifdef GCI_WINDOWS

    const std::string rm_cmd = "del";

#else

    const std::string rm_cmd = "rm";

#endif

/* change extension of file name */
#define CHANGE_EXT(file, ext) (file.substr(0, file.find('.') + 1) + ext)

namespace SysOp {

    /* compiles all .c files in directory */
    void compileFiles(std::string, std::string="");

    /* return list of all files in a directory with given extension */
    std::vector<std::string> getFiles(std::string, std::string);

    /* create a text file from a file stream */
    void createTextFile(FILE*, std::string);

    /* return a list of named (no '.') sections in the object file */
    std::vector<std::string> getNamedSections(std::string);

    /* create a string by concatenating each element of the vector,
       appending prefix before each string */
    std::string concatVector(std::vector<std::string>, std::string="");

    /* rename sections in object file */
    void renameSections(std::string, std::string="");

    /* runs a command and waits for it to finish */
    int runCMD(std::string);

}

#endif
