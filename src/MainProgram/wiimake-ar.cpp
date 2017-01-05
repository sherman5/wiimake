#include "../HighLevel/HighLevel.h"
#include "../ArgumentParsing/Parser.h"
#include "../ArgumentParsing/Arguments.h"
#include "Description.h"

#include <iostream>

int main(int argc, const char** argv)
{
    /* get command line arguments */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* parse meta options */
    CMDparser::parseMetaOptions(tokens);

    /* check for correct number of arguments */
    if (tokens.size() < 2)
    {
        throw std::invalid_argument("incorrect number of options");
        std::cout << Description::usage << std::endl;
        exit(0);
    }

    /* parse command line arguments */
    Arguments args;
    args.name = tokens.front();
    args.sources = TokenList(tokens.begin() + 1, tokens.end());

    /* create library */
    Builder::buildLibrary(args);

    /* return without error */
    return 0;
}
