#include "LowLevel.h"

#include <string>
#include <iostream>
#include <algorithm>

/* runs cmd and waits for it to finish */
int System::runCMD(std::string cmd, bool display)
{
    #ifdef WIIMAKE_WINDOWS
        std::replace(cmd.begin(), cmd.end(), '/', '\\');
    #endif

    FILE* cmd_ex = popen(cmd.c_str(), "r");
    pclose(cmd_ex);

    if (display)
    {
        std::cout << cmd << std::endl;
    }
}







