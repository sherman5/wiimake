#ifndef MEMORY_CONFIG_H
#define MEMORY_CONFIG_H

#include <utility>
#include <vector>
#include <stdint.h>
#include <string>

typedef std::pair<uint32_t, uint32_t> MemRegion;
typedef std::vector<MemRegion> RegionList;

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
        MemRegion operator*()

    };

/********************* END OF ITERATOR CLASS ***************/

private:

    /* address to inject code at */
    uint32_t mInjectAddress;

    /* instruction being overridden at this address */
    uint32_t mOriginalInstruction;

    /* list of all available memory regions */
    RegionList mRegions;

public:

    /* default constructor */    
    MemoryConfig();

    /* construct from file */    
    MemoryConfig(std::string);

    /* add memory region */    
    void AddRegion(std::string, std::string);

    /* get iterator to beginning of memory region list */
    iterator begin();

    /* get iterator to end of memory region list */
    iterator end();

    /* setters */
    void setInjectAddress(std::string);
    void setOriginalInstruction(std::string);

    /* getters */    
    uint32_t injectAddress();
    uint32_t originalInstruction();

};

#endif

