#include "../HighLevel/HighLevel.h"
#include "../ArgumentParsing/Parser.h"
#include "../ArgumentParsing/Arguments.h"
#include "Description.h"

#include <iostream>

void run(TokenList& tokens)
{
   /* check for correct number of arguments */
    if (tokens.size() < 2)
    {
        throw std::invalid_argument("incorrect number of options");
    }

    /* parse command line arguments */
    Arguments args;
    
    /* get flags */
    while (tokens.front().find("-") != std::string::npos)
    {
        args.compileFlags.push_back(tokens.front());
        tokens.erase(tokens.begin());
    }

    /* get name and sources */
    args.name = tokens.front();
    args.sources = TokenList(tokens.begin() + 1, tokens.end());

    /* create library */
    Builder::buildLibrary(args);
}

int main(int argc, const char** argv)
{
    try
    {
        /* get command line arguments */
        TokenList tokens = CMDparser::getTokens(argc, argv);

        /* parse meta options */
        CMDparser::parseMetaOptions(tokens);

        /* run program */
        run(tokens);
    }
    catch (std::exception& e)
    {
        /* handle error */
        std::cout << e.what() << std::endl;
        std::cout << Description::usage << std::endl;
        return 1;
    }

    /* return without error */
    return 0;
}
