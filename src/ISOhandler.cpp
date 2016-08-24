#include <fstream>
#include <vector>
#include <stdint.h>
#include <endian.h>
#include <utility>
#include <string>

#include <iostream>

#include "ISOhandler.h"
#include "RegionFileParser.h"

/* constructor, store path to iso file */
ISOhandler::ISOhandler(std::string path_to_iso) {

    m_iso_path = path_to_iso;

}

/* get the DOL offset of a 32-bit RAM address */
uint32_t ISOhandler::GetDOLoffset(uint32_t ram_addr) {

    uint32_t dol_offset = 0;

    for (int i = 0; i < 9; i++) {

        /* find corresponding region of DOL */
        if (ram_addr >= addr_key[i].second) {

            /* calculate RAM offset and add to DOL address to get DOL offset */
            dol_offset = addr_key[i].first + ram_addr - addr_key[i].second;

        }

    }

    return dol_offset;

}

/* write 32-bit value to RAM address */
void ISOhandler::IsoWrite(uint32_t addr, uint32_t val) {

    /* open iso file */
    std::fstream iso_file (m_iso_path);

    /* find address of interest */
    iso_file.seekp(DOL_START + GetDOLoffset(addr));      

    /* account for endianess */
    val = be32toh(val);

    /* write value */
    iso_file.write(reinterpret_cast<char *>(&val), sizeof(val));

}

/* read 32-bit value from RAM address */
uint32_t ISOhandler::IsoRead(uint32_t addr) {

    /* open iso file */
    std::ifstream iso_file (m_iso_path);

    /* find address of interest */
    iso_file.seekg(DOL_START + GetDOLoffset(addr));      

    /* read line at addr */
    uint32_t line;
    iso_file.read(reinterpret_cast<char *>(&line), sizeof(line));

    return htobe32(line);

}

/* save the current code in the regions provided */
void ISOhandler::CreateRestorePoint(std::string region_file_path, std::string save_file_path) {

    /* data vector to write to file */
    std::vector<uint32_t> data;

    /* parse region file to find address of interest */
    RegionFileParser parser (region_file_path);

    /* add the address and instruction at injection point */
    uint32_t addr = parser.GetInjectionPoint();
    data.push_back(addr);
    data.push_back(IsoRead(addr));

    /* loop through remaining memory regions */
    RegionFileParser::iterator it = parser.begin();
    for (; !it.atEnd(); ++it) {
    
        addr = (*it).first;
        while (addr <= (*it).second) {

            /* store address and instruction at every line within region */
            data.push_back(addr);
            data.push_back(IsoRead(addr));

            /* go to next line of code */
            addr += 0x04;

        }            

    }

    /* write data to file */
    std::ofstream outfile(save_file_path, std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char *> (&data[0]), data.size() * sizeof(uint32_t));

}

/* load code from save file */
void ISOhandler::Restore(std::string load_file_path) {

    /* open up binary file for reading */
    std::ifstream infile(load_file_path, std::ios::in | std::ios::binary);

    /* find end of file */
    infile.seekg(0, infile.end);

    /* calculate number of 32-bit values */
    unsigned int length = infile.tellg() * sizeof(char) / sizeof(uint32_t);

    /* return to begining of file */
    infile.seekg(0, infile.beg);

    /* loop through values and write consecutive values as (addr, value) pairs */
    uint32_t addr, value;
    for (unsigned int i = 0; i < length; ++i) {

        infile.read(reinterpret_cast<char *>(&addr), sizeof(addr));
        infile.read(reinterpret_cast<char *>(&value), sizeof(addr));
        IsoWrite(addr, value);
        ++i;

    }

}

/* inject code into iso */
void ISOhandler::InjectCode(std::vector< std::pair<uint32_t, uint32_t> > code) {

    /* loop through code and write each (addr, value) pair */
    for (auto it = code.begin();; it != code.end(); ++it) {

        IsoWrite((*it).first, (*it).second);

    }

}


