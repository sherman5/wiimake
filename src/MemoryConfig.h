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

        /* this class just wraps an internal iterator */        
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

    /* address to inject code at */
    uint32_t m_inject_addr;

    /* instruction being overridden at this address */
    uint32_t m_inject_instruction;

    /* list of all available memory regions */
    RegionList m_regions;

public:

    /* default constructor */    
    MemoryConfig();

    /* construct from file */    
    MemoryConfig(std::string);

    /* add memory region */    
    void AddRegion(std::string, std::string);

    /* setters */
    void SetInjectAddress(std::string);
    void SetInjectInstruction(std::string);

    /* getters */    
    uint32_t GetInjectAddress();
    uint32_t GetInjectInstruction();

    /* get iterator to beginning of memory region list */
    iterator begin();

    /* get iterator to end of memory region list */
    iterator end();

};

#endif

