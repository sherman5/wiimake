#include "LowLevel.h"

void Linker::link(FileList& objects, std::string script,
std::string name, std::string entry, TokenList linkerFlags)
{
    /* put all object files and linker flags in individual strings */
    std::string objectNames, flags;
    for (auto& flg : linkerFlags) { flags += flg + " ";}
    for (auto& obj : objects) { objectNames += obj + " ";}

    /* run linker - output command */
    std::string cmd = "powerpc-eabi-ld -e " + entry + " " + flags +
        " -o " + name + " --script " + script + " " + objectNames;

    if (System::runCMD(cmd, true) != 0)
    {
        throw std::runtime_error("Linker Error");
    }
}
