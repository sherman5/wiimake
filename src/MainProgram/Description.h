#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include "Version.h"

namespace Description
{

    const std::string version = "\nwiimake " VERSION_MAJOR "."
        VERSION_MINOR "." VERSION_REVISION "\n";

    const std::string summary = "\n"
    "\t wiimake is an automated tool for compiling, allocating,\n"
    "\t linking, and injecting C code into games for the\n"
    "\t Wii/Gamecube. The user must provide a configuration file\n"
    "\t that contains information about the sources and libraries\n"
    "\t that should be injected, entry points of their code\n"
    "\t (function names), and which regions of RAM are available for\n"
    "\t overwriting. Given this information, the program proceeds as\n"
    "\t follows: (1) compile all source files (2) find sizes of each\n"
    "\t section of code that needs to be injected (3) find an\n"
    "\t arrangement of the code that fits within the memory regions\n"
    "\t provided by the user in the config file (4) run the linker (5)\n"
    "\t inject the code into the appropiate addresses. In one command the\n"
    "\t user can take their raw C files and produce a working iso\n\n\n"
    "\t wiimake-isotool is useful for operating on an iso file directly.\n"
    "\t The --read flag allows the user to read any RAM address from the\n"
    "\t iso, which is useful when trying to find the original instruction\n"
    "\t that is being overwritten at the injection point. The --checksum\n"
    "\t option allows for simple verification of the iso version. There\n"
    "\t is also --save and --load which allow for easy distribution\n"
    "\t of mods.\n";

    const std::string usage = "\nUsage:\n"
    "wiimake ISO CONFIGFILE [--save-temps]\n"
    "wiimake-isotool ISO [--save <file>] [--load <file>]\n"
    "                    [--read <addr>] [--checksum] \n";

    const std::string space = "                            ";

    const std::string help = "";

}

#endif
