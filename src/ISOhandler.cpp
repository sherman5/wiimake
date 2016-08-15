#include <fstream>
#include <vector>
#include <stdint.h>
#include <endian.h>
#include <utility>
#include <string>

#include <iostream>

#include "ISOhandler.h"
#include "TextFileParser.h"

ISOhandler::ISOhandler(std::string path_to_iso) {

    m_iso_path = path_to_iso;

}

uint32_t ISOhandler::GetDOLoffset(uint32_t ram_addr) {

    uint32_t dol_offset = 0;

    for (int i = 0; i < 9; i++) {

        if (ram_addr >= addr_key[i].second) {

            dol_offset = addr_key[i].first + ram_addr - addr_key[i].second;

        }

    }

    return dol_offset;

}

void ISOhandler::IsoWrite(uint32_t addr, uint32_t val) {

    std::fstream iso_file (m_iso_path);
    iso_file.seekp(0x1E800 + GetDOLoffset(addr));      

    val = be32toh(val);
    iso_file.write(reinterpret_cast<char *>(&val), sizeof(val));

}

uint32_t ISOhandler::IsoRead(uint32_t addr) {

    std::ifstream iso_file (m_iso_path);
    iso_file.seekg(0x1E800 + GetDOLoffset(addr));      

    uint32_t line;
    iso_file.read(reinterpret_cast<char *>(&line), sizeof(line));

    return htobe32(line);

}

void ISOhandler::CreateRestorePoint(std::string region_file_path, std::string save_file_path) {

    std::vector<uint32_t> data;
    TextFileParser parser (region_file_path, regionFile);

    uint32_t addr = parser.GetInjectionPoint();
    data.push_back(addr);
    data.push_back(IsoRead(addr));

    TextFileParser::iterator it = parser.begin();
    for (; !it.atEnd(); ++it) {
    
        addr = (*it).first;
        while (addr <= (*it).second) {

            data.push_back(addr);
            data.push_back(IsoRead(addr));
            addr += 0x04;

        }            

    }

    std::ofstream outfile(save_file_path, std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char *> (&data[0]), data.size() * sizeof(uint32_t));

}

void ISOhandler::Restore(std::string load_file_path) {

    std::ifstream infile(load_file_path, std::ios::in | std::ios::binary);

    infile.seekg(0, infile.end);
    unsigned int length = infile.tellg() * sizeof(char) / sizeof(uint32_t);
    infile.seekg(0, infile.beg);

    uint32_t addr, value;

    for (unsigned int i = 0; i < length; ++i) {

        infile.read(reinterpret_cast<char *>(&addr), sizeof(addr));
        infile.read(reinterpret_cast<char *>(&value), sizeof(addr));
        IsoWrite(addr, value);
        ++i;

    }

}

void ISOhandler::InjectCode(std::vector< std::pair<uint32_t, uint32_t> > code) {

    std::vector< std::pair<uint32_t, uint32_t> >::iterator it = code.begin();
    for (; it != code.end(); ++it) {

        IsoWrite((*it).first, (*it).second);

    }

}


