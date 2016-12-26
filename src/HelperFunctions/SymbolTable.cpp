#include "HelperFunctions.h"

#include <fstream>

void parseLine(std::ifstream& file, std::string line,
    std::vector<unsigned>& sizes)
{
    /* read value, store as int (assume we start at beginning of line) */
    file >> line;
    unsigned size = stoul(line, nullptr, 0);

    /* find the keyword "_size" or the start of the next line */
    while (line.find("_size") == std::string::npos
            || line.find(':') == std::string::npos)
    {
        file >> line;
    }

    /* if keyword found, store size is proper id */
    if (line.find("_size") != std::string::npos)
    {
        unsigned id = stoul(line.substr(8), nullptr, 0);
        sizes[id] = size;
    }
}

std::vector<unsigned> SymbolTable::getSizes(std::string fileName, int num)
{
    /* dump file contents to text */
    System::runCMD("powerpc-eabi-readelf -s " + fileName + " > sizes.txt");

    /* open up text file and read first two lines (skip title) */
    std::ifstream file = std::ifstream("sizes.txt");
    std::string line;
    file >> line; file >> line;

    /* declare vector of sizes */
    std::vector<unsigned> sizes;
    sizes.reserve(num);

    /* parse each line of the file */
    while (!file.eof())
    {
        /* look for start of line */
        if (line.find(':') != std::string::npos)
        {
            parseLine(file, line, sizes);
        }
    }

    /* return vector of sizes */
    return sizes;
}
