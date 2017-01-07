#ifndef SECTION_H
#define SECTION_H

#include <stdint.h>
#include <string>

/* section in object file */
struct Section
{
    /* path/to/objectfile (section name) */
    std::string path;
    
    /* size of section (size of asm code) */
    unsigned int size;

    /* address this section is injected to */
    uint32_t address;

    /* constructors */
    Section(std::string p) : path (p) {}
    Section(std::string p, uint32_t a) : path(p), address(a) {}

    /* used for sorting */
    bool operator<(const Section& other)
    {
        return size < other.size;
    }

    /* used for sorting */
    bool operator==(const Section& other)
    {
        return size == other.size;
    }
};

#endif
