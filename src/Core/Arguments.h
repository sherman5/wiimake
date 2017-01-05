#ifndef ARGUMENTS_H
#define ARGUMETNS_H

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
