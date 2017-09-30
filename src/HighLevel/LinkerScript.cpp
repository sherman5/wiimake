#include "HighLevel.h"

#include <fstream>

void LinkerScript::CreateSizeScript(SectionList& sections, std::string name, Arguments& args)
{
    /* open up text file, write first line */
    std::ofstream script (name, std::ios::out | std::ios::trunc);
    
    for (auto& sym : args.linkerSymbols)
    {
        script << sym.name << " = 0;" << std::endl;
    }
    script << "SECTIONS {" << std::endl;

    /* section number */
    unsigned int num = 1;

    /* iterate through all sections, write contents to script */
    for (auto& sect : sections)
    {
        script << "wiimake_section_" << std::dec << num << " :\n{\n\t"
            << sect.path << "\n}\n_sizeof_section_" << num <<
            "=SIZEOF(wiimake_section_" << num << ");\n";

        num++; //increment here to avoid warning -Wsequence-point
    }

    /* write final line of linker script and close file */
    script << "}" << std::endl;
    script.close();
}

void LinkerScript::CreateFinalScript(SectionList& sections, std::string name, Arguments& args)
{
    /* open up text file, write first line */
    std::ofstream script (name, std::ios::out | std::ios::trunc);
    for (auto& sym : args.linkerSymbols)
    {
        script << sym.name << " = 0x" << std::hex << sym.value << ";" << std::endl;
    }
    script << "SECTIONS {" << std::endl;
    
    /* section number */
    unsigned int num = 1;

    /* iterate through all sections, write contents to script */
    for (auto& sec : sections)
    {
        script << "wiimake_section_" << std::dec << num++ << " 0x" 
            << std::hex << sec.address << " : /* section size = 0x" 
            << sec.size << " */\n{\n\t" << sec.path << "\n}\n";
    }

    /* write final line of linker script and close file */
    script << "}" << std::endl;
    script.close();
}

