#include "Description.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

void Description::displayVersion()
{
    std::cout << "\nGCI v1.0\nDecember 26, 2016\n" << std::endl;
    exit(0);
}

void Description::displayDescription()
{
    std::cout << "\n"
    "\t GCI is an automated tool for compiling, allocating,\n"
    "\t linking, and injecting C code into games for the\n"
    "\t Wii/Gamecube. The main use of this program is with the\n"
    "\t \"--inject\" flag, which tells the program to do the\n"
    "\t following 4 steps: 1) compile code in the directory\n"
    "\t provided 2) find an arrangement of the code that will\n"
    "\t fit inside the memory regions provided 3) link the code\n"
    "\t 4) inject the code into the iso file provided. In one\n"
    "\t command the user can take their raw C files and produce\n"
    "\t a runnable iso file that has their _main() function\n"
    "\t injected at a specified address\n"
    << std::endl;
}                    

void Description::displayUsage()
{
    std::cout << "\nUsage: "
    "GCI [--mem-config <file>] [--iso-file <file>] [--save <file>]\n"
    "\t [--load <file>] [--inject <directory>] [--lib <library>]\n"
    "\t [--include <directory>] [--save-temps] [--ar <directory>]\n"
    "\t [--output <output>] [--read <address>] [--help] [--version]\n"
    "\t [--usage]\n" << std::endl;

    displayDescription();

    exit(0);
}

void Description::displayHelp()
{
    displayDescription();

    std::string space = "                            ";

    std::cout <<
    "    --inject <directory>    "
    "compile C code in <directory> and inject into iso\n"
    + space + "specified by --iso-file, only overwriting memory\n"
    + space + "regions specified by --mem-config\n"
    "    --iso-file <file>       "
    "specify which iso file to use\n"
    "    --mem-config <file>     "
    "specify a memory configuration file\n"
    "    --include <directory>   "
    "search for header files in <directory>\n"
    "    --lib <library>         "
    "link against <library>\n"
    "    --save-temps            "
    "don't delete temporary files created by injection\n"
    + space + "process\n"
    "    --ar <directory>        "
    "compile C code in <directory> and create static\n"
    + space + "library\n"
    "    --output <name>         "
    "specify library name to be <output>\n"
    "    --save <file>           "
    "save state of iso from --iso-file to <file>\n"
    "    --load <file>           "
    "restore iso in --iso-file to state saved in <file>\n"
    "    --read <address>        "
    "read memory at <address> in iso file specified\n"
    + space + "with --iso-file\n"
    "    --help                  "
    "display this help message\n"
    "    --usage                 "
    "display usage message\n"
    "    --version               "
    "display program version\n"
    << std::endl;

    exit(0);
}

