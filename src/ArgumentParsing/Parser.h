#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <fstream>

#include "Arguments.h"

typedef std::vector<std::string> TokenList;

namespace CMDparser
{
    /* return a list of tokens from the command line arguments */
    TokenList getTokens(int, const char**);

    /* parse command line options */
    void parseMetaOptions(TokenList&);

    /* check if user passed in option */
    bool optionExists(TokenList&, std::string);
};

namespace ConfigParser
{
    /* parse config file */
    void parse(Arguments&);

    /* convert file to vector of strings */
    TokenList getTokens(Arguments&);

    /* remove all comments from config file */
    void removeComments(TokenList&);

    /* store all variables in file */
    void storeAllVariables(TokenList&, Arguments&);

    /* store a single variable */
    void storeVariable(Arguments&, std::string, TokenList);

    /* store the memory regions variable */
    void storeMemRegions(Arguments&, TokenList);
};

#endif
