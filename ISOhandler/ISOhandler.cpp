#include <fstream>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <endian.h>
#include <utility>

#include "ISOhandler.hpp"

ISOhandler::ISOhandler(std::string name) {

  iso_name = name;

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

  std::fstream iso_file ("../ISO/" + iso_name + ".iso");
  iso_file.seekp(0x1E800 + GetDOLoffset(addr));      

  val = be32toh(val);
  iso_file.write(reinterpret_cast<char *>(&val), sizeof(val));

}

uint32_t ISOhandler::IsoRead(uint32_t addr) {

  std::ifstream iso_file ("../ISO/" + iso_name + ".iso");
  iso_file.seekg(0x1E800 + GetDOLoffset(addr));      

  uint32_t line;
  iso_file.read(reinterpret_cast<char *>(&line), sizeof(line));

  return htobe32(line);

}

void ISOhandler::CleanISO() {

  std::ifstream in_file;
  std::ifstream region_file("../CodeRegions/regions.txt");  
  
  std::string lead_addr, line;
  uint32_t addr;

  while (region_file >> line) {

    lead_addr = line.substr(0, line.find('-'));
    addr = std::stoul(lead_addr, nullptr, 16);
    in_file.open("../CodeRegions/" + lead_addr + ".txt");
    
    while (in_file >> line) {

      IsoWrite(addr, std::stoul(line, nullptr, 16));
      addr += 0x04;

    }

    in_file.close();

  }

  //TODO: add injection point to input file
  IsoWrite(0x80377998, 0x7EE3BB78); //injection point, original code

}

