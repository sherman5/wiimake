#include "HelperFunctions.h"

#include <fstream>

bool lineOfCode(std::string line)
{
    /* check if not section, symbol, or file name */
    return line.find(".") == std::string::npos
           && line.find(">") == std::string::npos
           && line.find("_") == std::string::npos
           && line.find(":") != std::string::npos;
}

std::pair<uint32_t, uint32_t> getLine(std::string line, std::ifstream& file)
{
    /* current line is address */
    uint32_t address = std::stoul(line, nullptr, 16);    

    /* get first byte of instruction */
    std::string instruction;
    file >> instruction;

    /* add remaining 3 bytes */
    for (unsigned i = 0; i < 3; ++i)
    {
        file >> line;
        instruction += line;
    }

    /* return (address, instruction) */
    return std::make_pair(address, stoul(instruction, nullptr, 16));
}

ASMcode ObjectFile::extractASM(std::string fileName)
{
    /* vector of code to return */
    ASMcode code;

    /* dump file contents to text */
    System::runCMD("powerpc-eabi-objdump -D " + fileName + " > exec.txt");

    /* open up text file and read first line */
    std::ifstream file = std::ifstream("exec.txt");
    std::string line;
    file >> line;

    /* loop until end of file or unnamed section */
    while (!file.eof() && line.find("section .") == std::string::npos)
    {
        /* check if valid line of code */
        if (lineOfCode(line))
        {
            /* remove ':' */
            line.pop_back();

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
    //TODO: make this general, not specific to this list of
    //section names

    /* create objcopy command */
    std::string cmd = "powerpc-eabi-objcopy";
    cmd += " --rename-section .text=text" + id;
    cmd += " --rename-section .rodata=rodata" + id;
    cmd += " --rename-section .sdata=sdata" + id;
    cmd += " --rename-section .data=data" + id;
    cmd += " --rename-section .gnu.attributes=attr" + id;
    cmd += " " + file;

    /* execute command */
    System::runCMD(cmd);
}

std::vector<std::string> ObjectFile::getNamedSections(std::string file)
{


}


