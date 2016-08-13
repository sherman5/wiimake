#include <fstream>
#include <vector>
#include <stdint.h>
#include <endian.h>
#include <utility>

#include "ISOhandler.h"

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

    MemoryList restore_point;
    std::ifstream region_file(region_file_path);

    std::string line, first_addr, last_addr;
    uint32_t addr;

    region_file >> line;
    addr = std::stoul(line, nullptr, 16);
    restore_point.push_back(std::make_pair(addr, IsoRead(addr)));

    while (region_file >> line) {

        first_addr = line.substr(0, line.find('-'));
        last_addr = line.substr(line.find('-') + 1, line.length());

        addr = std::stoul(first_addr, nullptr, 16);

        while (addr <= std::stoul(last_addr, nullptr, 16)) {
                
            restore_point.push_back(std::make_pair(addr, IsoRead(addr)));
            addr += 0x04;

        }

    }

    std::ofstream outfile (save_file_path);

}

void ISOhandler::Restore() {

}


