#include "LowLevel.h"

#include <fstream>

/* check if at beginning of valid line of code */
bool ObjectFile::lineOfCode(std::string line)
{
    /* check if not section, symbol, or file name */
    return line.find(".") == std::string::npos
           && line.find(">") == std::string::npos
           && line.find("_") == std::string::npos
           && line.find(":") != std::string::npos;
}

/* parse a line of code from text file, return (address, instruction)
   - requires object file to have named sections containing '_' */
std::pair<uint32_t, uint32_t> ObjectFile::getLine(std::string line,
std::ifstream& file)
{
    /* remove ':' */
    line.pop_back();

    /* remaining line is address */
    uint32_t address = std::stoul(line, nullptr, 16);    

    /* next 4 lines are the 4 bytes of the instruction */
    std::string instruction;

    for (unsigned i = 0; i < 4; ++i)
    {
        file >> line;
        instruction += line;
    }

    /* return (address, instruction) */
    return std::make_pair(address, stoul(instruction, nullptr, 16));
}

/* extract asm from binary file */
ASMcode ObjectFile::extractASM(std::string fileName)
{
    /* vector of code to return */
    ASMcode code;

    /* dump file contents to text */
    System::runCMD("powerpc-eabi-objdump -D " + fileName + " > "
        + CHANGE_EXT(fileName, "txt"));

    /* open up text file and read first line */
    std::ifstream file (CHANGE_EXT(fileName, "txt"), std::ios::in);
    std::string line;

    /* loop until end of file or comment section */
    while (!file.eof() && line != ".comment:")
    {
        /* check if valid line of code */
        if (lineOfCode(line))
        {
            /* add line to code */
            code.push_back(getLine(line, file));
        }

        /* get next line */
        file >> line;
    }

    /* close file */
    file.close();

    /* return assembly code */
    return code;
}

/* rename sections in object file since sections with '.' are ignored, id
   makes names unique */
void ObjectFile::renameSections(std::string file, std::string id)
{
    /* delete comment section */
    System::runCMD("powerpc-eabi-objcopy -R .comment " + file);

    std::vector<std::string> sections = ObjectFile::getNamedSections(file);

    /* create objcopy command */
    std::string cmd = "powerpc-eabi-objcopy";
    for (auto& sec : sections)
    {
        sec.erase(0,1);
        cmd += " --rename-section ." + sec + "=" + sec + id;
    }
    cmd += " " + file;

    /* execute command */
    System::runCMD(cmd);
}

/* returns all section names without '.' */
std::vector<std::string> ObjectFile::getNamedSections(std::string fileName)
{
    /* list of named sections */
    std::vector<std::string> sections;

    /* dump file contents to text */
    System::runCMD("powerpc-eabi-objdump -D " + fileName + " > temp.txt");
    
    /* open up text file */ 
    std::ifstream file ("temp.txt", std::ios::in);
    std::string line;

    /* read every line of file, look out for sections */
    while (!file.eof())
    {
        file >> line;

        /* section found */
        if (line == "section")
        {
            file >> line;  
            sections.push_back(line.substr(0, line.find(":")));
        }
    }

    /* close file */
    file.close();

    /* remove temp file */
    System::runCMD(System::rm + " temp.txt");

    /* return section list */
    return sections;
}


