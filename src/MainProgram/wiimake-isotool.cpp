#include "Parser.h"

#include <iostream>

int main(unsigned argc, const char** argv)
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
    
    /* get iso file */
    Arguments args;
    args.isoFile = tokens[1];

    /* run the program */
    if (tokens.front() == "--save")
    {
        args.name = tokens.back();
        ISO::saveState(args);
    }
    else if (tokens.front() == "--load")
    {
        args.name = tokens.back();
        ISO::loadState(args);
    }
    else if (tokens.front() == "--read")
    {
        std::cout << ISO::read(tokens.back(), args);
    }
    else
    {
        throw std::invalid_argument("invalid option: " + tokens.front());
    }

    /* return without error */
    return 0;
}
