#ifndef SETUP_H
#define SETUP_H

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(GCI_WINDOWS)

    #define GCI_WINDOWS
    
#endif

#include <vector>
#include <utility>
#include <string>
#include <fstream>

/* section in object file */
struct Section
{
    std::string path;
    unsigned int size;
    uint32_t address;

    Section(std::string p) : path (p) {}

    bool operator<(const Section& other)
    {
        return size < other.size;
    }

    bool operator==(const Section& other)
    {
        return size == other.size;
    }
};



/* memory layout for code to be injected to */
struct MemoryConfig
{
    std::vector< std::pair<uint32_t, uint32_t> > regions;
    std::pair<uint32_t, uint32_t> injectPoint;

    MemoryConfig() {}
    MemoryConfig(std::string path)
    {
        /* open up text file */    
        std::ifstream file (path);

        /* string to hold current line, ints to hold (begin, end) */
        std::string s;
        uint32_t b, e;

        /* store each region */
        while (file >> s)
        {
            b = stoul(s.substr(0, s.find('-')), nullptr, 16);
            e = stoul(s.substr(s.find('-') + 1, s.length()), nullptr, 16);
            regions.push_back(std::make_pair(b, e));
        }
        
        /* last region is actually inject point information */
        injectPoint = regions.back();
        regions.pop_back();

        /* close file */
        file.close();
    }
};

#endif
