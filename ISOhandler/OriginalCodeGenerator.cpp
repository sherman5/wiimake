#include <iostream>
#include <fstream>
#include <stdint.h>
#include <endian.h>
#include <vector>

#include "ISOhandler.hpp"

int main() {

  ISOhandler iso("CustomMelee");
  
  std::ifstream region_file("../CodeRegions/regions.txt");
  std::ofstream out_file;
  
  std::string line, first_addr, last_addr;
  uint32_t addr;

  while (region_file >> line) {

    first_addr = line.substr(0, line.find('-'));
    last_addr = line.substr(line.find('-') + 1, line.length());

    out_file.open("../CodeRegions/" + first_addr + ".txt");
    addr = std::stoul(first_addr, nullptr, 16);

    while (addr <= std::stoul(last_addr, nullptr, 16)) {

      out_file << std::hex << iso.IsoRead(addr) << std::endl;      
      addr += 0x04;

    }
    
    out_file.close();

  }



}

