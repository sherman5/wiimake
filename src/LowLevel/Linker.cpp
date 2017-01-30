#include "LowLevel.h"

void Linker::link(std::string script, std::string name, TokenList flags)
{
    /* put all object files and linker flags in individual strings */
    std::string linkerFlags;
    for (auto& flg : flags) { linkerFlags += flg + " ";}

    /* run linker - output command */
    std::string cmd = "powerpc-eabi-ld " + linkerFlags +
        " -o " + name + " --script " + script;

    if (System::runCMD(cmd, true) != 0)
    {
        throw std::runtime_error("Linker Error");
    }
}
