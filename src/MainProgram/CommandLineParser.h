#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <vector>
#include <string>

struct Arguments
{
    /* main option */    
    int inject, save, load, ar, read;

    /* argument for main option */
    std::string mainArg;
    
    /* secondary options */
    std::string memFile, isoPath, outputName;
    int saveTemps;

    /* lib and include paths */
    std::vector<std::string> libs, includePaths;

    /* constructor */
    Arguments() : inject(0), save(0), load(0), ar(0), read(0), saveTemps(0)
        {}
};

namespace CMDparser {

    /* check if user passed in option */
    bool optionExists(std::vector<std::string>, std::string);

    /* get argument associated with option */
    std::string getArg(std::vector<std::string>&, std::string);

    /* process command line args and store in struct */
    void storeArgs(std::vector<std::string>, Arguments&);

    /* parse command line args and store in Arguments struct */
    void parse(int, char**, Arguments&);

};

#endif
