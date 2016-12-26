#include "HelperFunctions.h"

#include <sstream>

void Linker::link(SectionList& sections, std::string script,
    std::string name)
{
    /* strings for storing info */
    std::string lastFile, allFiles;
    
    /* get list of file names */
    for (auto it = sections.begin(); it != sections.end(); ++it)
    {
        /* get name of section */
        std::stringstream ss((*it).path);
        std::string sectionName;
        ss >> sectionName;
    
        /* check if name is unique */        
        if (sectionName != lastFile)
        {
            lastFile = sectionName;
            allFiles += sectionName + " ";
        }
    }

    /* run linker */
    System::runCMD("powerpc-eabi-ld -e _main -o " + name + " --script " +
                    script + " " + allFiles);
}
