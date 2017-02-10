#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdint.h>
#include <vector>
#include <string>
#include <utility>
#include <cmath>

/* single region of memory (struct used for sorting) */
struct MemRegion
{
    uint32_t start, end;

    MemRegion(uint32_t a, uint32_t b) : start(a), end(b)
        {align();}

    MemRegion(std::string a, std::string b) :
        start(stoul(a, nullptr, 16)), end(stoul(b, nullptr, 16))
        {align();}
    
    void align()
    {
        start = 4 * floor((double) start / 4.0) + 4;
        end = 4 * floor((double) end / 4.0) - 4;
    }

    bool operator<(const MemRegion& other)
    {
        return end - start < other.end - other.start;
    }

    bool operator==(const MemRegion& other)
    {
        return end - start == other.end - other.start;
    }
};

struct FixedSymbol
{
    std::string name = "";
    uint32_t address = 0, instruction = 0;

    FixedSymbol(std::string n, uint32_t a, uint32_t i) : 
        name(n), address(a), instruction(i) {}
};

struct Arguments
{
    /* configuration file */
    std::string configFile;

    /* fixed symbols */
    std::vector<FixedSymbol> fixedSymbols;

    /* source and lib files, include paths */
    std::vector<std::string> sources, libs, includePaths;

    /* compiler/linker flags */
    std::vector<std::string> compileFlags, linkFlags;

    /* available memory regions */
    std::vector<MemRegion> memRegions;

    /* static overwrites to make */
    std::vector< std::pair<uint32_t, uint32_t> > staticOverwrites;
};

#endif
