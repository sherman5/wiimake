#include "LowLevel.h"

#include <sstream>
#include <iostream>

//TODO: throw exception with linker error
void Linker::link(SectionList& sections, std::string script,
std::string name, std::string entry, TokenList linkerFlags)
{
    /* strings for storing info */
    std::string allFiles, sectionName;
    
    /* get list of file names */
    for (auto& section : sections)
    {
        /* get first token of section path (name) */
        std::stringstream ss(section.path);
        ss >> sectionName;
    
        /* check if name is unique */        
        if (allFiles.find(sectionName) == std::string::npos)
        {
            allFiles += sectionName + " ";
        }
    }

    /* get all flags */
    std::string flags;
    for (auto& flag : linkerFlags)
    {
        flags += flag + " ";
    }

    /* run linker - output command */
    std::string cmd = "powerpc-eabi-ld -e " + entry + " " + flags +
        " -o " + name + " --script " + script + " " + allFiles;

    if(System::runCMD(cmd, true) != 0)
    {
        throw std::runtime_error("Linker Error");
    }
}
