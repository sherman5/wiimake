#include "LowLevel.h"

void Linker::link(std::string script, std::string name, TokenList flags,
std::string entry)
{
    /* put all object files and linker flags in individual strings */
    std::string linkerFlags;
    for (auto& flg : flags) {linkerFlags += " " + flg + " ";}

    if (!entry.empty()) {entry = " -e " + entry + " ";}

    /* run linker - output command */
    std::string cmd = "powerpc-eabi-ld" + entry + linkerFlags +
        " -o " + name + " --script " + script;

    RUNTIME_ERROR(System::runCMD(cmd, true) != 0, "Linker Error");
}
