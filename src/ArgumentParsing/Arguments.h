#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "../IsoHandling/ISO.h"

/* single region of memory (struct used for sorting) */
struct MemRegion
{
    uint32_t start, end;

    MemRegion(uint32_t a, uint32_t b) : start(a), end(b) {}

    MemRegion(std::string a, std::string b) :
        start(stoul(a, nullptr, 16)), end(stoul(b, nullptr, 16)) {}
    
    bool operator<(const MemRegion& other)
    {
        return end - start < other.end - other.start;
    }

    bool operator==(const MemRegion& other)
    {
        return end - start == other.end - other.start;
    }
};

struct Arguments
{
    /* name of library/save/load file */
    std::string name;

    /* configuration file */
    std::string configFile;
    
    /* ISO object to handl iso file */
    ISO isoFile;

    /* entry symbol */
    std::string entry;

    /* save temporary files */
    bool saveTemps;

    /* address to read from iso */
    std::string address;

    /* inject point */
    uint32_t injectAddress, originalInstruction;
    
    /* source and lib files, include paths */
    std::vector<std::string> sources, libs, includePaths;

    /* compiler/linker flags */
    std::vector<std::string> compileFlags, linkFlags;

    /* available memory regions */
    std::vector<MemRegion> memRegions;
};

#endif
