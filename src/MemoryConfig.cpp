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

    /* open up text file */    
    std::ifstream file (path);

    /* read first line (contains inject address and instruction */
    std::string line;
    file >> line;

    /* set injection address */    
    SetInjectAddress(line.substr(0, line.find('-')));

    /* set injection instruction */    
    SetInjectInstruction(line.substr(line.find('-') + 1, line.length()));

    /* loop through remaining lines */
    while (file >> line) {

        /* get beginning and end of region */
        std::string begin = line.substr(0, line.find('-'));
        std::string end = line.substr(line.find('-') + 1, line.length());

        /* add to memory region list */
        AddRegion(begin, end);

    }

}

/* add memory region */
void MemoryConfig::AddRegion(std::string begin, std::string end) {

    m_regions.push_back(std::make_pair(stoul(begin, nullptr, 16),
                                       stoul(end, nullptr, 16)));
   
}

/* get iterator to beginning of memory region list */
MemoryConfig::iterator MemoryConfig::begin() {

    return MemoryConfig::iterator(m_regions.begin());

}

/* get iterator to end of memory region list */
MemoryConfig::iterator MemoryConfig::end() {

    return MemoryConfig::iterator(m_regions.end());

}

/**** setters ****/

void MemoryConfig::SetInjectAddress(std::string addr) {

    /* convert string (hex) to int */
    m_inject_addr = stoul(addr, nullptr, 16);

}

void MemoryConfig::SetInjectInstruction(std::string inst) {

    /* convert string (hex) to int */
    m_inject_instruction = stoul(inst, nullptr, 16);

}

/**** getters ****/

uint32_t MemoryConfig::GetInjectAddress() {

    return m_inject_addr;

}

uint32_t MemoryConfig::GetInjectInstruction() {

    return m_inject_instruction;

}

