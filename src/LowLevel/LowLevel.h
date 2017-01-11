#ifndef LOW_LEVEL_H
#define LOW_LEVEL_H

/* define symbol if on windows */
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(WIIMAKE_WINDOWS)

    #define WIIMAKE_WINDOWS
    
#endif

/* change extension of file name */
#define CHANGE_EXT(file, ext) (std::string(file).substr(0, std::string(file).find_last_of('.') + 1) + ext)

#include "../ArgumentParsing/Parser.h"
#include "Section.h"

#include <vector>
#include <fstream>
#include <utility>
#include <stdint.h>

typedef std::vector<std::string> FileList;
typedef std::vector<Section> SectionList;
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;

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

    /* runs a command and waits for it to finish */
    int runCMD(std::string, bool=false);
}

/* parse symbol table */
namespace SymbolTable
{
    /* parse single line of table - record section size */
    void parseLine(std::ifstream&, std::string, std::vector<unsigned>&);
    
    /* gret sizes of all sections in this symbol table */
    std::vector<unsigned> getSizes(std::string, int);
}

/* manipulate and parse object files */
namespace ObjectFile
{
    /* get all code from object file */
    ASMcode extractASM(std::string);

    /* get list of named sections in object file */
    std::vector<std::string> getNamedSections(std::string);

    /* name relevant sections in object file */
    void renameSections(std::string, std::string="");

    /* get single line of code from object file */
    std::pair<uint32_t, uint32_t> getLine(std::string, std::ifstream&);

    /* verify current line is beginning of line of code */
    bool lineOfCode(std::string);

}

/* compile lists of C files */
namespace Compiler
{
    /* compile files in directory, using include paths given */
    FileList compile(FileList&, TokenList = TokenList(),
        FileList = FileList());
}

/* link lists of object files */
namespace Linker
{
    /* link all sections, using linker script provided */
    void link(SectionList&, std::string, std::string,
        std::string, TokenList = TokenList());
}  

#endif
