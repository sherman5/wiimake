#include "Global.h"
#include "ISO.h"

#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

/* constructor from file path */
ISO::ISO(std::string path)
{
    /* open up file stream */
    mFile = new std::fstream(path, std::ios::in | std::ios::out
        | std::ios::binary);

    /* find DOL start */
    mFile->seekg(0x0420);
    mFile->read(reinterpret_cast<char*>(&mStartDOL), sizeof(uint32_t));
    mStartDOL = HOST_TO_BE(mStartDOL);

    /* populate DOL table */
    uint32_t dol, ram, size;
    for (uint32_t offset = 0; offset < 0x47; offset += 0x04)
    {
        /* get dol offset */
        mFile->seekg(mStartDOL + offset);
        mFile->read(reinterpret_cast<char*>(&dol), sizeof(uint32_t));
    
        /* get ram address */
        mFile->seekg(mStartDOL + offset + 0x48);
        mFile->read(reinterpret_cast<char*>(&ram), sizeof(uint32_t));

        /* get section size */
        mFile->seekg(mStartDOL + offset + 0x90);
        mFile->read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

        /* add section if non-zero size */
        if (size > 0)
        {
            mDOLtable.push_back(IsoSection(HOST_TO_BE(dol),
                HOST_TO_BE(ram), HOST_TO_BE(size)));
        }
    }

    /* sort DOL table on RAM addresses */
    std::sort(mDOLtable.begin(), mDOLtable.end());

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

/* find DOL offset corresponding to RAM address */
uint32_t ISO::dolOffset(uint32_t address) const
{
    /* check if pure iso offset */
    if (address < 0x80000000)
    {
        return address;
    }

    /* find section that contains this address */
    unsigned i = 0;
    while (address > mDOLtable[i].RAMaddress + mDOLtable[i].size) { ++i;} 

    /* return offset */
    return mStartDOL + mDOLtable[i].DOLoffset
        + address - mDOLtable[i].RAMaddress;
}

/* read 32-bit address */
uint32_t ISO::read(uint32_t address) const
{
    /* check that address is valid */
    if (address > mCodeEnd)
    {
        throw std::invalid_argument("iso read: RAM address out of range");
    }
    else if (address < 0x80000000)
    {
        std::cout << "\n" << std::hex << address << " is too low for RAM,"
            " will be interpreted\nas a pure offset in the iso file\n"
            << std::endl;
    }

    /* put stream at correct address */
    mFile->seekg(dolOffset(address));

    /* read 32-bits */
    uint32_t value;
    mFile->read(reinterpret_cast<char *>(&value), sizeof(uint32_t));
    
    /* return value, account for endianess */
    return HOST_TO_BE(value);
}
    
/* read 32-bit address */
uint32_t ISO::read(std::string address) const
{
    return read(stoul(address, nullptr, 16));
}

/* write 32-bit value to 32-bit RAM address */
void ISO::write(uint32_t address, uint32_t value)
{
    /* put stream at correct address */
    mFile->seekp(dolOffset(address));

    /* account for endianess */    
    value = BE_TO_HOST(value);

    /* write value */
    mFile->write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
}

/* save the current state of the iso file */
void ISO::saveState(std::string fileName) const
{
    /* data vector to write to file */
    std::vector<uint32_t> data;

    /* first and last address - store at front of data */
    data.push_back(mCodeStart);
    data.push_back(mCodeEnd);
    
    /* loop through all code */
    for (uint32_t addr = mCodeStart; addr <= mCodeEnd; addr += 0x04)
    {
        /* store instruction */
        data.push_back(read(addr));
    }
    
    /* open save file */
    std::ofstream saveFile(fileName, std::ios::out | std::ios::binary
        | std::ios::trunc);

    /* write data to file */
    saveFile.write(reinterpret_cast<char *> (&data[0]), data.size()
        * sizeof(uint32_t));

    /* close file */
    saveFile.close();
}

/* load code from save file */
void ISO::loadState(std::string fileName)
{
    /* open up binary file for reading */
    std::ifstream loadFile (fileName, std::ios::in | std::ios::binary);
    
    /* first and last address, temp value for reading */
    uint32_t start, end, value;
    loadFile.read(reinterpret_cast<char *>(&start), sizeof(uint32_t));
    loadFile.read(reinterpret_cast<char *>(&end), sizeof(uint32_t));

    /* loop through stored values */
    for (uint32_t addr = start; addr <= end; addr += 0x04)
    {
        /* read in values as (addr, instruction) pairs */
        loadFile.read(reinterpret_cast<char *>(&value), sizeof(uint32_t));
       
        /* write value to iso */    
        write(addr, value);
    }

    /* close file */
    loadFile.close();
}

/* inject code into iso */
void ISO::injectCode(const std::vector< std::pair<uint32_t, uint32_t> >&
code)
{
    /* loop through code and write each (address, value) pair */
    for (auto& line : code)
    {
        write(line.first, line.second);
    }
}

/* calculate check sum of iso file */
uint64_t ISO::checkSum()
{
    uint64_t sum = 0, addr = 0;
    uint32_t word = 0;
    
    mFile->seekg(0, std::ios::beg);

    while (mFile->read(reinterpret_cast<char*>(&word), sizeof(word)))
    {
        sum += HOST_TO_BE(word) * addr;
        addr += 4;
    }
    return sum;
}

