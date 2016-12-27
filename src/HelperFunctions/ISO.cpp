#include "HelperFunctions.h"
#include "ByteOrder.h"

#include <stdint.h>
#include <vector>
#include <fstream>

/* get address table info from configuration file */
Table ISO::addressTable(std::string configFile, int gameNum)
{
    /* table to store info */
    Table table;
        
    /* open up text file */
    std::ifstream file (configFile);
    std::string line;

    /* read until correct game number found */
    while (!file.eof() && line != "game_id:" + gameNum)
    {
        file >> line;
    }

    /* make sure game is found */
    if (!file.eof())
    {
        throw std::invalid_argument("game ID not found");
    }

    /* read the DOL start address */
    file >> line; file >> line;
    table.push_back(std::make_pair(stoul(line, nullptr, 16), 0));

    /* read each (DOL,RAM) entry */
    file >> line; file >> line;

    /* read until next game section */
    while (line != "**")
    {
        /* get DOL value, skip token, get RAM value */
        uint32_t d = stoul(line, nullptr, 16);
        file >> line; file >> line;
        uint32_t r = stoul(line, nullptr, 16);
        
        /* store value in table, read next line */
        table.push_back(std::make_pair(d, r)):
        file >> line;
    }

    /* close file */
    file.close()

    /* return table */
    return table;
}

/* get the DOL offset of a 32-bit RAM address */
uint32_t ISO::DOLoffset(uint32_t ramAddress)
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

