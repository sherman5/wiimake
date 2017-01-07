#include "HighLevel.h"

#include <fstream>

void LinkerScript::CreateTempScript(SectionList& sections, std::string name)
{
    /* open up text file */
    std::ofstream script (name, std::ios::out | std::ios::trunc);

    /* write first line of linker script */
    script << "SECTIONS {" << std::endl;

    /* section number */
    unsigned int num = 1;

    /* iterate through all sections */
    for (auto it = sections.begin(); it != sections.end(); ++it)
    {
        /* write section header */
        script << "gci_" << std::dec << num << " :\n{\n\t" << (*it).path
            << "\n}\n";

        /* save size in symbol */
        script << "_sizeof_gci_" << num << "=SIZEOF(gci_" << num << ");\n";

        /* increment section number */
        num++;
    }

    /* write final line of linker script */
    script << "}" << std::endl;

    /* close linker script */
    script.close();
}

void LinkerScript::CreateFinalScript(SectionList& sections, std::string name)
{
   /* open up text file */
    std::ofstream script (name, std::ios::out | std::ios::trunc);

    /* write first line of linker script */
    script << "SECTIONS {" << std::endl;
    
    /* section number */
    unsigned int num = 1;

    /* iterate through all sections */
    for (auto it = sections.begin(); it != sections.end(); ++it)
    {
        /* write section header for each section */
        script << "gci_" << std::dec << num << " 0x" << std::hex <<
            (*it).address << " :\n{\n\t" << (*it).path << "\n}\n";

        /* increment section number */
        num++;
    }

    /* write final line of linker script */
    script << "}" << std::endl;
    
    /* close linker script */
    script.close();
}

