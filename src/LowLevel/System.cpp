#include "LowLevel.h"

#ifdef WIIMAKEWINDOWS

    #include <windows.h>
    #include <process.h>

#endif

#include <string>
#include <iostream>
#include <algorithm>

/* runs cmd and waits for it to finish */
int System::runCMD(std::string cmd, bool display)
{
    #ifdef WIIMAKE_WINDOWS
        std::replace(cmd.begin(), cmd.end(), '/', '\\');
    #endif

    if (display) { std::cout << cmd << std::endl;}
    else { cmd += " 2>&1";} //send stderr to stdout

    FILE* cmdExe = popen(cmd.c_str(), "r");
    return pclose(cmdExe);
}







