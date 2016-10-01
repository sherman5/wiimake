#include "MemoryConfig.h"

#include <fstream>

/********************* ITERATOR CLASS **********************/

/* store internal iterator */
MemoryConfig::iterator::iterator(RegionList::iterator it) {

    m_iter = it;

}

/* increment to next line */    
MemoryConfig::iterator MemoryConfig::iterator::operator++() {

    ++m_iter;

}

/* standard post-increment operator */        
MemoryConfig::iterator MemoryConfig::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

/* compare iterators */
bool MemoryConfig::iterator::operator!=(const MemoryConfig::iterator& other) {

    return m_iter != other.m_iter;

}

/* return MemRegion (begin, end) */
MemRegion MemoryConfig::iterator::operator*() {

    return *m_iter;

}

/********************* END OF ITERATOR CLASS ***************/

/* default constructor */
MemoryConfig::MemoryConfig() {}

/* construct from file */
MemoryConfig::MemoryConfig(std::string path) {

    std::ifstream file (path);

    std::string line;
    file >> line;

    m_inject_addr = std::stoul(line.substr(0, line.find('-')), nullptr, 16);
    m_inject_instruction = std::stoul(line.substr(line.find('-') + 1, line.length()), nullptr, 16);

    while (file >> line) {

        std::string first = line.substr(0, line.find('-'));
        std::string second = line.substr(line.find('-') + 1, line.length());
        m_regions.push_back(std::make_pair(stoul(first, nullptr, 16), stoul(second, nullptr, 16)));

    }

}

void MemoryConfig::SetInjectAddress(uint32_t addr) {

    m_inject_addr = addr;

}

void MemoryConfig::SetInjectInstruction(uint32_t inst) {

    m_inject_instruction = inst;

}

void MemoryConfig::AddRegion(uint32_t begin, uint32_t end) {

    m_regions.push_back(std::make_pair(begin, end));
   
}

uint32_t MemoryConfig::GetInjectAddress() {

    return m_inject_addr;

}

uint32_t MemoryConfig::GetInjectInstruction() {

    return m_inject_instruction;

}

MemoryConfig::iterator MemoryConfig::begin() {

    return MemoryConfig::iterator(m_regions.begin());

}

MemoryConfig::iterator MemoryConfig::end() {

    return MemoryConfig::iterator(m_regions.end());

}

