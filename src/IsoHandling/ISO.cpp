#include "ISO.h"

#include <stdint.h>
#include <vector>
#include <fstream>

/* constructor from file path */
ISO::ISO(std:string path)
{
    /* open up file stream */
    mFile = new std::ifstream(path, std::ios::in | std::ios::out
        | std::ios::binary);

    /* find DOL start */
    mFile.seekg(0x0420);
    mFile.read(reinterpret_cast<char *>(&mStartDOL), sizeof(mStartDOL));

    /* populate DOL table */
    uint32_t dol, ram, size;
    for (uint32_t offset = 0; offset < 0x47; offset += 0x4)
    {
        /* get dol offset */
        mFile.seekg(mStartDOL + offset);
        mFile.read(reinterpret_cast<char *>(dol), sizeof(dol));
    
        /* get ram address */
        mFile.seekg(mStartDOL + offset + 0x48);
        mFile.read(reinterpret_cast<char *>(ram), sizeof(ram));

        /* get section size */
        mFile.seekg(mStartDOL + offset + 0x90);
        mFile.read(reinterpret_cast<char *>(size), sizeof(size));

        /* add section if non-zero size */
        if (size > 0)
        {
            mDOLtable.push_back(IsoSection(dol, ram, size));
        }
    }

    /* get code start and end */
    mCodeStart = mDOLtable.front().RAMaddress;
    mCodeEnd = mDOLtable.back().RAMaddress + mDOLtable.back().size;
}

/* destructor - close file stream */
ISO::~ISO()
{
    /* close file stream */
    mFile->close();
    delete mFile;
}

/* place file stream at RAM address */
void ISO::gotoAddress(uint32_t address)
{
    /* find section that contains this address */
    unsigned i = 0;
    while (address > mDOLtable[i].RAMaddress + mDOLtable[i].size) { ++i;} 

    /* put file stream at correct address */
    mFile.seekg(mStartDOL + mDOLtable[i].DOLoffset
        + address - mDOLtable[i].RAMaddress);
}

/* read 32-bit address */
uint32_t ISO::read(uint32_t address)
{
    /* put stream at correct address */
    gotoAddress(address);

    /* read 32-bits */
    uint32_t value;
    mFile.read(reinterpret_cast<char *>(value), sizeof(value));
    
    /* return value, account for endianess */
    return HOST_TO_BE(value);
}
    
/* read 32-bit address */
uint32_t ISO::read(std::string address)
{
    return read(stoul(address, nullptr, 16));
}

/* write 32-bit value to 32-bit RAM address */
void ISO::write(uint32_t address, uint32_t value)
{
    /* put stream at correct address */
    gotoAddress(address);

    /* account for endianess */    
    value = BE_TO_HOST(value);

    /* write value */
    mFile.write(reinterpret_cast<char *>(&value), sizeof(value));
}

/* save the current state of the iso file */
void ISO::saveState(std:string saveFile)
{
    /* data vector to write to file */
    uint32_t value;
    std::vector<uint32_t> data;

    /* first and last address - store at front of data */
    data.push_back(mCodeStart);
    data.push_back(mCodeEnd);

    /* put stream at correct address */
    gotoAddress(mCodeStart);
    
    /* loop through all code */
    for (uint32_t addr = mCodeStart; addr <= mCodeEnd; addr += 0x04)
    {
        /* read value */
        mFile.read(reinterpret_cast<char *>(&value), sizeof(value));

        /* store instruction */
        data.push_back(HOST_TO_BE(value));
    }
    
    /* open save file */
    std::ofstream saveFile(saveFile, std::ios::out | std::ios::binary
        | std::ios::trunc);

    /* write data to file */
    saveFile.write(reinterpret_cast<char *> (&data[0]), data.size()
        * sizeof(uint32_t));

    /* close file */
    saveFile.close();
}

/* load code from save file */
void ISO::loadState(std::string loadFile)
{
    /* open up binary file for reading */
    std::ifstream loadFile (loadFile, std::ios::in | std::ios::binary);
    
    /* first and last address, temp variable for reading */
    uint32_t start, end, value;
    loadFile.read(reinterpret_cast<char *>(&start), sizeof(start));
    loadFile.read(reinterpret_cast<char *>(&end), sizeof(end));

    /* put stream at start address */
    gotoAddress(start);

    /* loop through stored values */
    for (uint32_t addr = start; addr <= end; addr += 0x04)
    {
        /* read in values as (addr, instruction) pairs */
        loadFile.read(reinterpret_cast<char *>(&value), sizeof(value));
   
        /* account for endianess */
        value = BE_TO_HOST(value);
    
        /* write value to iso */    
        iso.write(reinterpret_cast<char *>(&value), sizeof(value));
    }

    /* close file */
    loadFile.close();
}

/* inject code into iso */
void ISO::injectCode(ASMcode& code)
{
    /* loop through code and write each (address, value) pair */
    for (auto it = code.begin(); it != code.end(); ++it)
    {
        write((*it).first, (*it).second, args);
    }
}

