#include "LowLevel.h"

#include <sstream>
#include <iostream>

//TODO: throw exception with linker error
void Linker::link(SectionList& sections, std::string script,
    std::string name)
{
    /* strings for storing info */
    std::string allFiles, sectionName;
    
    /* get list of file names */
    for (auto it = sections.begin(); it != sections.end(); ++it)
    {
        /* get first token of section path (name) */
        std::stringstream ss((*it).path);
        ss >> sectionName;
    
        /* check if name is unique */        
        if (allFiles.find(sectionName) == std::string::npos)
        {
            allFiles += sectionName + " ";
        }
    }

    /* run linker - output command */
    std::string cmd = "powerpc-eabi-ld -e _main -o " + name + " --script "
        + script + " " + allFiles;

    System::runCMD(cmd, true);
}
