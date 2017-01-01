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
    /* check if valid address */
    if (addr < args.configOptions["code_start"]
        || addr > args.configOptions["code_end"])
    {
        throw std::invalid_argument("read at invalid address");
    }

    /* open iso file */
    std::ifstream isoFile (args.cmdOptions["--iso-file"]);

    /* find address of interest */
    isoFile.seekg(args.configOptions["DOL_start"]
        + ISO::DOLoffset(addr, args.addressTable));      

    /* read line at addr */
    uint32_t line;
    isoFile.read(reinterpret_cast<char *>(&line), sizeof(line));

    /* close iso file */
    isoFile.close();    

    /* return value, accounting for endianess */    
    return HOST_TO_BE(line);
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
    val = BE_TO_HOST(val);

    /* write value */
    iso.write(reinterpret_cast<char *>(&val), sizeof(val));

    /* close iso file */
    iso.close();
}

/* save the current code from the regions provided */
void ISO::saveState(Arguments& args)
{
    /* open iso file - faster than calling read */
    std::ifstream isoFile (args.cmdOptions["--iso-file"]);

    /* data vector to write to file */
    uint32_t value;
    std::vector<uint32_t> data;

    /* start address - store at front of data */
    uint32_t address = args.configOptions["code_start"];
    data.push_back(address);

    /* loop through all code */
    for (; address <= args.configOptions["code_end"]; address += 0x04)
    {
        /* find address */    
        isoFile.seekg(args.configOptions["DOL_start"]
        + ISO::DOLoffset(address, args.addressTable)); 

        /* read value */
        isoFile.read(reinterpret_cast<char *>(&value), sizeof(value));

        /* store instruction */
        data.push_back(HOST_TO_BE(value));
    }
    
    /* open write file */
    std::ofstream outfile(args.cmdOptions["--save"],
        std::ios::out | std::ios::binary);

    /* write data to file */
    outfile.write(reinterpret_cast<char *> (&data[0]),
        data.size() * sizeof(uint32_t));

    /* close output file */
    outfile.close();
}

/* load code from save file */
void ISO::loadState(Arguments& args)
{
    /* open iso file - faster than calling write */
    std::fstream iso (args.cmdOptions["--iso-file"]);

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
    uint32_t addr, value;

    /* read start address */
    infile.read(reinterpret_cast<char *>(&addr), sizeof(addr));        

    /* loop through stored values */
    for (unsigned int i = 1; i < length; ++i)
    {
        /* read in values as (addr, instruction) pairs */
        infile.read(reinterpret_cast<char *>(&value), sizeof(value));

        /* find address */
        iso.seekp(args.configOptions["DOL_start"]
            + DOLoffset(addr, args.addressTable));

        /* account for endianess */
        value = BE_TO_HOST(value);
    
        /* write value to iso */    
        iso.write(reinterpret_cast<char *>(&value), sizeof(value));

        /* increment address */
        addr += 0x04;
    }

    /* close input file */
    infile.close();
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

