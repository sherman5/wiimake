#ifndef DESCRIPTION_H
#define DESCRIPTION_H

namespace Description
{

    const std::string version = "\nGCI v1.0\nDecember 30, 2016\n";

    const std::string summary = "\n"
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
    "\t injected at a specified address\n";

    const std::string usage = "\nUsage: "
    "GCI [--inject <directory>] [--save <file>] [--load <file>]\n"
    "\t [--ar <directory>] [--read <address>] [--config-file <file>]\n"
    "\t [--game-id <id>] [--iso-file <file>] [--lib <library>]\n"
    "\t [--include <directory>] [--output <filename>] [--save-temps]\n"
    "\t [--help] [--usage] [--version]\n";

    const std::string space = "                            ";

    const std::string help =
    "    --inject <directory>    "
    "compile C code in <directory> and inject into iso\n"
    + space + "specified by --iso-file, only overwriting memory\n"
    + space + "regions specified by --mem-config\n"
    
    "    --save <file>           "
    "save state of iso from --iso-file to <file>\n"

    "    --load <file>           "
    "restore iso in --iso-file to state saved in <file>\n"

    "    --ar <directory>        "
    "compile C code in <directory> and create static\n"
    + space + "library\n"

    "    --read <address>        "
    "read memory at <address> in iso file specified\n"
    + space + "with --iso-file\n"

    "    --config-file <file>    "
    "specify a the configuration file\n"

    "    --game-id <id>          "
    "id of game in the configuration file\n"

    "    --iso-file <file>       "
    "specify which iso file to use\n"

    "    --lib <library>         "
    "link against <library>\n"

    "    --include <directory>   "
    "search for header files in <directory>\n"

    "    --output <filename>     "
    "specify library name to be <output>\n"

    "    --save-temps            "
    "don't delete temporary files created by injection\n"
    + space + "process\n"

    "    --help                  "
    "display this help message\n"

    "    --usage                 "
    "display usage message\n"

    "    --version               "
    "display program version\n";

};

#endif
