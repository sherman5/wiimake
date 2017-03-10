#include "LowLevel.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cmath>

/* get sizes of each section in binary file (needs special symbols) */
std::vector<unsigned> SymbolTable::getSizes(std::string fileName, int num)
{
    /* dump file contents to text, open file */
    System::runCMD("powerpc-eabi-readelf -s " + fileName + " > sizes.txt");
    std::ifstream file ("sizes.txt", std::ios::in);

    /* declare vector of sizes */
    std::vector<unsigned> sizes;
    sizes.resize(num);

    /* parse each line of the file */
    std::string size, id, line;
    while (!file.eof())
    {
        /* if keyword found, store size in proper id */
        std::getline(file, line);
        if (line.find("_sizeof_section_") != std::string::npos)
        {
            size = line.substr(line.find(":")).substr(2, 8);
            id = line.substr(line.find("_size")).substr(16,
                std::string::npos);
            sizes[stoul(id, nullptr, 0) - 1] = stoul(size, nullptr, 16);
        }
    }

    /* round up sizes not divisble by 4 */
    for (int i = 0; i < num; ++i)
    {
        unsigned rounded = 4 * floor((double) sizes[i] / 4.0);
        if (rounded != sizes[i])
        {
            sizes[i] = rounded + 4;
        }
    }

    /* close file, return vector of sizes */
    file.close();
    return sizes;
}
