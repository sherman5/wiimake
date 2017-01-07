#include "LowLevel.h"

#include <fstream>
#include <iostream>

/* file stream starts and ends at beginning of line */
void SymbolTable::parseLine(std::ifstream& file, std::string line,
    std::vector<unsigned>& sizes)
{
    /* verify start at beginning of line */
    if (line.find(":") == std::string::npos)
    {
        throw std::invalid_argument("not at start of line in symbol table");
    }

    /* read value, store as int (assume we start at beginning of line) */
    file >> line;
    unsigned size = stoul(line, nullptr, 16);

    /* find the keyword "_sizeof" or the start of the next line */
    while (!file.eof() && line.find("_sizeof") == std::string::npos
            && line.find(':') == std::string::npos)
    {
        file >> line;
    }

    /* if keyword found, store size in proper id */
    if (line.find("_sizeof") != std::string::npos)
    {
        unsigned id = stoul(line.substr(12), nullptr, 0);
        sizes[id - 1] = size;
        file >> line;
    }
}

/* get sizes of each section in object file (needs special symbols) */
std::vector<unsigned> SymbolTable::getSizes(std::string fileName, int num)
{
    /* dump file contents to text */
    System::runCMD("powerpc-eabi-readelf -s " + fileName + " > sizes.txt");

    /* open up text file */
    std::ifstream file ("sizes.txt", std::ios::in);

    /* read until first line */
    std::string line;
    while (line != "0:")
    {
        file >> line;
    }   

    /* declare vector of sizes */
    std::vector<unsigned> sizes;
    sizes.resize(num);

    /* parse each line of the file */
    while (!file.eof())
    {
        SymbolTable::parseLine(file, line, sizes);
    }

    /* return vector of sizes */
    return sizes;
}
