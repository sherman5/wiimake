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
    "\t that should be injected, as well as the entry point of the\n"
    "\t code (function name). In this file, the user must also specify\n"
    "\t which regions of RAM are available for overwriting, as well\n"
    "\t as an address where entry point of the code should be injected.\n"
    "\t Given this information, the program proceeds as follows:\n"
    "\t (1) compile all source files (2) find sizes of each section of\n"
    "\t code that needs to be injected - object files from sources and\n"
    "\t libraries (3) find an arrangement of the code that fits within\n"
    "\t the memory regions provided by the user in the config file\n"
    "\t (4) run the linker (5) inject the code into the appropiate\n"
    "\t addresses. In one command the user can take their raw C files\n"
    "\t and produce a runnable iso file that has their main function\n"
    "\t injected at a specified address.\n\n\n"
    "\t wiimake-ar is a tool for creating static libraries. It works\n"
    "\t very similiar to the usual powerpc-eabi-ar command, except\n"
    "\t it adds section flags so that the library can be broken down\n"
    "\t into smaller pieces for injection. This makes it easier to find\n"
    "\t an arrangement that fits in the memory regions available.\n\n\n"
    "\t wiimake-isotool is useful for operating on an iso file directly.\n"
    "\t The --read flag allows the user to read any RAM address from the\n"
    "\t iso, which is useful when trying to find the original instruction\n"
    "\t that is being overwritten at the injection point. There is also\n"
    "\t --save and --load which allow for easy distribution of mods.\n";

    const std::string usage = "\nUsage:\n"
    "wiimake ISO CONFIGFILE [--save-temps]\n"
    "wiimake-ar LIBNAME file1 file2 ...\n"
    "wiimake-isotool ISO [--save <file>] [--load <file>] [--read <addr>]\n";

    const std::string space = "                            ";

    const std::string help = "";

}

#endif
