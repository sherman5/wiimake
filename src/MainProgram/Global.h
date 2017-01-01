#ifndef GLOBAL_H
#define GLOBAL_H

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(GCI_WINDOWS)

    #define GCI_WINDOWS
    
#endif

/* change extension of file name */
#define CHANGE_EXT(file, ext) (std::string(file).substr(0, std::string(file).find_last_of('.') + 1) + ext)

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>

/* section in object file */
struct Section
{
    std::string path;
    unsigned int size;
    uint32_t address;

    Section(std::string p) : path (p) {}

    Section(std::string p, uint32_t a) : path(p), address(a) {}

    /* used for sorting */
    bool operator<(const Section& other)
    {
        return size < other.size;
    }

    bool operator==(const Section& other)
    {
        return size == other.size;
    }
};

/* convenient typedef's */
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;
typedef std::vector< std::pair<uint32_t, uint32_t> > Table;
typedef std::vector<std::string> FileList;
typedef std::vector<Section> SectionList;

/* single region of memory (struct used for sorting) */
struct MemRegion
{
    uint32_t start, end;

    MemRegion(uint32_t a, uint32_t b) : start(a), end(b) {}
    
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
    /* --inject, --save, --load, --ar, --read, --config-file,
       --iso-file, --output, --save-temps, --game-id,
       --help, --usage, --version
    */
    std::map<std::string, std::string> cmdOptions;

    /* code_start, code_end, inject_address, original_instruction,
       DOL_start
    */
    std::map<std::string, uint32_t> configOptions;

    /* libs and include paths */
    std::vector<std::string> libs, includePaths;

    /* available memory regions */
    std::vector<MemRegion> memRegions;
 
    /* address table */
    Table addressTable;
};

#endif
