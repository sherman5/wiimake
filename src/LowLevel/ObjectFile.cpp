#include "LowLevel.h"

#include <fstream>
#include <string>

/* break up object file into list of strings (tokens) */
TokenList ObjectFile::getTokens(std::string fileName)
{
    /* dump file contents to text */
    System::runCMD("powerpc-eabi-objdump -D " + fileName + " > temp.txt");
    
    /* read every line of file */
    std::ifstream file ("temp.txt", std::ios::in);
    std::string line;
    TokenList tokens;

    /* read whole file */
    while (file.peek() != EOF)
    {
        file >> line;
        tokens.push_back(line);
        file >> std::ws; //discard white space (so it can see EOF)
    }

    /* close and delete file */
    file.close();
    System::runCMD(System::rm + " temp.txt");

    /* return tokens */
    return tokens;
}

/* check if at beginning of valid line of code */
bool ObjectFile::lineOfCode(std::string line)
{
    /* check if not section, symbol, or file name */
    return line.find(".") == std::string::npos
           && line.find(">") == std::string::npos
           && line.find("_") == std::string::npos
           && line.find(":") != std::string::npos;
}

/* parse a line of code from text file, return (address, instruction) */
std::pair<uint32_t, uint32_t> ObjectFile::getCode(TokenList::iterator& it)
{
    /* current position is address, next 4 lines are the instruction */
    std::string address = (*it).substr(0, (*it).find(":"));
    std::string instruction = *(it+1) + *(it+2) + *(it+3) + *(it+4);
    it += 4;

    /* return unsigned int versions of (address, instruction) */
    return std::make_pair(stoul(address, nullptr, 16),
        stoul(instruction, nullptr, 16));
}

/* extract asm from binary file */
ASMcode ObjectFile::extractASM(std::string fileName)
{
    /* get list of tokens in object file */
    TokenList tokens = ObjectFile::getTokens(fileName);

    /* get all asm code */
    ASMcode code;
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (lineOfCode(*it))
        {
            code.push_back(ObjectFile::getCode(it));
        }
    }
    
    /* dump file contents to text, let Builder decide to delete it */
    System::runCMD("powerpc-eabi-objdump -D " + fileName + " > "
        + CHANGE_EXT(fileName, "txt"));

    /* return assembly code */
    return code;
}

/* removes all un-needed sections in object file */
void ObjectFile::removeSections(std::string file)
{
    /* delete comment section */
    System::runCMD("powerpc-eabi-objcopy -R .comment " + file);
}

/* returns all section names in object file */
TokenList ObjectFile::getSections(std::string fileName)
{
    /* get list of tokens in object file */
    TokenList sections, tokens = ObjectFile::getTokens(fileName);

    /* find all section names */
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (*it == "section")
        {
            ++it;
            sections.push_back((*it).substr(0, (*it).find(":")));
        }
    }
    return sections;
}


