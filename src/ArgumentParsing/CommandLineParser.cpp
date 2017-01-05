#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Parser.h"
#include "../MainProgram/Description.h"

/* return a list of tokens from the command line arguments */
TokenList CMDparser::getTokens(unsigned argc, const char** argv)
{
    /* store each argument in vector */ 
    TokenList tokens;
    for (unsigned i = 1; i < argc; ++i)
    {
        tokens.push_back(std::string(argv[i]));
    }

    /* return vector of parsed tokens */
    return tokens;
}

bool optionExists(TokenList& tokens, std::string option)
{    
    return std::find(tokens.begin(), tokens.end(), option)
        != tokens.end();
}

void CMDparser::parseMetaOptions(TokenList& tokens)
{
    /* check for help/usage/version */
    if (CMDparser::optioExists(tokens, "--help"))
    {
        std::cout << Description::summary << std::endl;
        std::cout << Description::help << std::endl;
        exit(0);
    }
    else if (CMDparser::optioExists(tokens, "--usage"))
    {
        std::cout << Description::usage << std::endl;
        exit(0);
    }
    else if (CMDparser::optioExists(tokens, "--version"))
    {
        std::cout << Description::version << std::endl;
        exit(0);
    }
}
