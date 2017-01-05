#include "../ArgumentParsing/Parser.h"
#include "../IsoHandling/ISO.h"
#include "Description.h"

#include <iostream>

int main(int argc, const char** argv)
{
    /* get command line arguments */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* parse meta options */
    CMDparser::parseMetaOptions(tokens);

    /* check for correct number of arguments */
    if (tokens.size() != 3)
    {
        throw std::invalid_argument("incorrect number of arguments");
        std::cout << Description::usage << std::endl;
        exit(0);
    }
    
    /* create iso handler */
    ISO iso (tokens[1]);
    
    /* run the program */
    if (tokens.front() == "--save")
    {
        iso.saveState(tokens.back());
    }
    else if (tokens.front() == "--load")
    {
        iso.loadState(tokens.back());
    }
    else if (tokens.front() == "--read")
    {
        std::cout << iso.read(tokens.back());
    }
    else
    {
        throw std::invalid_argument("invalid option: " + tokens.front());
    }

    /* return without error */
    return 0;
}
