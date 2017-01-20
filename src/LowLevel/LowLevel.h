#ifndef LOW_LEVEL_H
#define LOW_LEVEL_H

#include "../Global.h"

#include <vector>
#include <fstream>
#include <utility>
#include <stdint.h>

/* run system specific commands */
namespace System
{
    /* commands used for copying/deleting files */
    #ifdef WIIMAKE_WINDOWS
        const std::string rm = "del";
        const std::string cp = "copy";
    #else
        const std::string rm = "rm";
        const std::string cp = "cp";
    #endif

    /* runs a command and waits for it to finish, option to display */
    int runCMD(std::string, bool=false);
}

/* parse symbol table */
namespace SymbolTable
{
    /* get sizes of each section in binary file (needs special symbols) */
    std::vector<unsigned> getSizes(std::string, int);
}

/* manipulate and parse object files */
namespace ObjectFile
{
    /* break up object file into list of strings (tokens) */
    TokenList getTokens(std::string);

    /* extract asm from binary file */
    ASMcode extractASM(std::string);

    /* returns all section names in object file */
    TokenList getSections(std::string);

    /* removes all un-needed sections in object file */
    void removeSections(std::string);

    /* parse a line of code from text file, return (address, instruction) */
    std::pair<uint32_t, uint32_t> getCode(TokenList::iterator&);

    /* check if at beginning of valid line of code */
    bool lineOfCode(std::string);
}

/* compile C files */
namespace Compiler
{
    /* compile single .c file, use include paths given */
    std::string compile(std::string, TokenList = TokenList(),
        FileList = FileList());

    /* compile files in directory, use include paths given */
    FileList compileAll(FileList&, TokenList = TokenList(),
        FileList = FileList());
}

/* link lists of object files */
namespace Linker
{
    /* link all sections, using linker script provided */
    void link(FileList&, std::string, std::string,
        std::string, TokenList = TokenList());
}  

#endif
