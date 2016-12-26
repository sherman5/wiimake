#include "HelperFunctions.h"

#include <stdint.h>
#include <vector>
#include <fstream>

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

    #include <winsock2.h>
    #include <sys/param.h>

    #if BYTE_ORDER == LITTLE_ENDIAN

        #define be32toh(x) ntohl(x)
        #define htobe32(x) htonl(x)

    #elif BYTE_ORDER == BIG_ENDIAN

        #define htobe32(x) (x)
        #define be32toh(x) (x)

    #endif

#else

    #include <endian.h>

#endif

//TODO: add config options for any game

#define DOL_START 0x1E800 //specific to Melee v1.02

/* map of DOL - RAM (specific to Melee v1.02) */
const std::pair<uint32_t, uint32_t> addressTable[10] = {

    {0x000100, 0x80003100},
    {0x3B3E20, 0x80005520},
    {0x3B3FC0, 0x800056C0},
    {0x002520, 0x80005940},
    {0x3B4240, 0x803B7240},
    {0x3B4260, 0x803B7260},
    {0x3B4280, 0x803B7280},
    {0x3B6840, 0x803B9840},
    {0x42E6C0, 0x804D36A0},
    {0x4313C0, 0x804D79E0}

};

/* get the DOL offset of a 32-bit RAM address */
uint32_t DOLoffset(uint32_t ramAddress)
{
    /* initialize return variable */
    uint32_t offset = 0;

    /* loop through each DOL section */
    for (int i = 0; i < 9; i++)
    {
        /* find corresponding region of DOL */
        if (ramAddress >= addressTable[i].second)
        {
            /* calculate RAM offset and add to DOL address
               to get DOL offset */
            offset = addressTable[i].first + ramAddress
                        - addressTable[i].second;
        }
    }

    return offset;
}

/* read 32-bit value from RAM address */
uint32_t ISO::read(std::string path, uint32_t address)
{
    /* open iso file */
    std::ifstream isoFile (path);

    /* find address of interest */
    isoFile.seekg(DOL_START + DOLoffset(address));      

    /* read line at addr */
    uint32_t line;
    isoFile.read(reinterpret_cast<char *>(&line), sizeof(line));

    /* return value, accounting for endianess */    
    return htobe32(line);
}

/* save the current code from the regions provided */
void ISO::saveState(std::string path, MemoryConfig& mem,
std::string saveFile)
{
    /* data vector to write to file */
    std::vector<uint32_t> data;

    /* add the address and instruction at injection point */
    uint32_t address = mem.injectPoint.first;
    data.push_back(address);
    data.push_back(read(path, address));

    /* loop through memory regions */
    for (auto it = mem.regions.begin(); it != mem.regions.end(); ++it)
    {
        /* get the first address in the region */
        address = (*it).first;

        /* while addr is still within region */
        while (address <= (*it).second) 
        {
            /* store address and instruction */
            data.push_back(address);
            data.push_back(read(path, address));

            /* go to next line of code */
            address += 0x04;
        }            
    }

    /* open write file */
    std::ofstream outfile(saveFile, std::ios::out | std::ios::binary);

    /* write data to file */
    outfile.write(reinterpret_cast<char *> (&data[0]),
                    data.size() * sizeof(uint32_t));
}

/* load code from save file */
void ISO::loadState(std::string path, std::string loadFile)
{
    /* open up binary file for reading */
    std::ifstream infile(loadFile, std::ios::in | std::ios::binary);

    /* find end of file */
    infile.seekg(0, infile.end);

    /* calculate number of 32-bit values */
    unsigned int length = infile.tellg() * sizeof(char) / sizeof(uint32_t);

    /* return to begining of file */
    infile.seekg(0, infile.beg);

    /* variables to read info into */
    uint32_t address, value;

    /* loop through stored values */
    for (unsigned int i = 0; i < length; i += 2)
    {
        /* read in values as (addr, instruction) pairs */
        infile.read(reinterpret_cast<char *>(&address), sizeof(address));
        infile.read(reinterpret_cast<char *>(&value), sizeof(address));

        /* write instructions to iso */
        injectCode(path, ASMcode(1, std::make_pair(address, value)));
    }
}

/* inject code into iso */
void ISO::injectCode(std::string isoPath, ASMcode code)
{
    /* open iso file */
    std::fstream iso (isoPath);

    /* variable to hold value at an address */
    uint32_t value;

    /* loop through code and write each (address, value) pair */
    for (auto it = code.begin(); it != code.end(); ++it)
    {
        /* find address */
        iso.seekp(DOL_START + DOLoffset((*it).first));      

        /* account for endianess */
        value = be32toh((*it).second);

        /* write value */
        iso.write(reinterpret_cast<char *>(&value), sizeof(value));
    }
}

