#include "HelperFunctions.h"
#include "ByteOrder.h"
#include "../MainProgram/Global.h"

#include <stdint.h>
#include <vector>
#include <fstream>

/* get the DOL offset of a 32-bit RAM address */
uint32_t ISO::DOLoffset(uint32_t ramAddress, Table& table)
{
    /* initialize return variable */
    uint32_t offset = 0;

    /* loop through each DOL section */
    for (auto it = table.begin(); it != table.end(); ++it)
    {
        /* find corresponding region of DOL */
        if (ramAddress >= (*it).second)
        {
            /* add RAM offset to DOL address to get DOL offset */
            offset = (*it).first + (ramAddress - (*it).second);
        }
    }

    /* return DOL offset */
    return offset;
}

/* read 32-bit value from RAM address */
uint32_t ISO::read(uint32_t addr, Arguments& args)
{
    /* open iso file */
    std::ifstream isoFile (args.cmdOptions["--iso-file"]);

    /* find address of interest */
    isoFile.seekg(args.configOptions["DOL_start"]
        + ISO::DOLoffset(addr, args.addressTable));      

    /* read line at addr */
    uint32_t line;
    isoFile.read(reinterpret_cast<char *>(&line), sizeof(line));

    /* return value, accounting for endianess */    
    return htobe32(line);
}

/* read 32-bit value from RAM address */
uint32_t ISO::read(std::string addr, Arguments& args)
{
    /* convert string to uint32_t */
    return ISO::read(stoul(addr, nullptr, 16), args);
}

/* write 32-bit value to RAM address */
void ISO::write(uint32_t addr, uint32_t val, Arguments& args)
{
    /* open iso file */
    std::fstream iso (args.cmdOptions["--iso-file"]);

    /* find address */
    iso.seekp(args.configOptions["DOL_start"]
        + DOLoffset(addr, args.addressTable));      

    /* account for endianess */
    val = be32toh(val);

    /* write value */
    iso.write(reinterpret_cast<char *>(&val), sizeof(val));
}

/* save the current code from the regions provided */
void ISO::saveState(Arguments& args)
{
    /* data vector to write to file */
    std::vector<uint32_t> data;

    /* start address */
    uint32_t address = args.configOptions["code_start"];

    /* loop through all code */
    for (; address <= args.configOptions["code_end"]; address += 0x04)
    {
        /* store (address, instruction) */
        data.push_back(address);
        data.push_back(ISO::read(address, args));
    }

    /* open write file */
    std::ofstream outfile(args.cmdOptions["--save"],
        std::ios::out | std::ios::binary);

    /* write data to file */
    outfile.write(reinterpret_cast<char *> (&data[0]),
        data.size() * sizeof(uint32_t));
}

/* load code from save file */
void ISO::loadState(Arguments& args)
{
    /* open up binary file for reading */
    std::ifstream infile(args.cmdOptions["--load"],
        std::ios::in | std::ios::binary);

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
        ISO::write(address, value, args);
    }
}

/* inject code into iso */
void ISO::injectCode(ASMcode& code, Arguments& args)
{
    /* loop through code and write each (address, value) pair */
    for (auto it = code.begin(); it != code.end(); ++it)
    {
        ISO::write((*it).first, (*it).second, args);
    }
}

