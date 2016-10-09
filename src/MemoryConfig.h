#ifndef MEMORY_CONFIG_H
#define MEMORY_CONFIG_H

#include <utility>
#include <vector>
#include <stdint.h>
#include <string>

typedef std::pair<uint32_t, uint32_t> MemRegion;
typedef std::vector< std::pair<uint32_t, uint32_t> > RegionList;

class MemoryConfig {

/********************* ITERATOR CLASS **********************/

public:

    class iterator {

    private:

        RegionList::iterator m_iter;

    public:

        /* open file stream and go to line containing first memory region */
        iterator(RegionList::iterator);

        /* increment to next line */    
        iterator operator++();

        /* standard post-increment operator */        
        iterator operator++(int);

        /* compare iterators */
        bool operator!=(const iterator&);

        /* return MemRegion (begin, end) */
        MemRegion operator*();

    };

/********************* END OF ITERATOR CLASS ***************/

private:

    uint32_t m_inject_addr;
    uint32_t m_inject_instruction;
    RegionList m_regions;

public:

    MemoryConfig();
    MemoryConfig(std::string);

    void SetInjectAddress(std::string);
    void SetInjectInstruction(std::string);
    void AddRegion(std::string, std::string);

    uint32_t GetInjectAddress();
    uint32_t GetInjectInstruction();

    iterator begin();
    iterator end();

};

#endif

