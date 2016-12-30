#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Parser.h"
#include "Description.h"

/* parse command line args and store in Arguments struct */
void CMDparser::parse(int argc, const char** argv, Arguments& args)
{
    /* store each CMD line arg in vector of strings */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* parse vector of args */
    CMDparser::parseOptions(tokens, args);
    CMDparser::parseLibsAndIncludes(tokens, args);

    /* check if any options remain */
    if (!tokens.empty())
    {
        throw std::invalid_argument("unrecognized option: "
            + tokens.front());
    }
}

/* return a list of tokens from the command line arguments */
TokenList CMDparser::getTokens(int argc, const char** argv)
{
    /* store each argument in vector */ 
    TokenList tokens;
    for (int i = 1; i < argc; ++i)
    {
        /* break up arguments at equals sign */
        std::string s (argv[i]);
        if (s.find("=") != std::string::npos)
        {
            tokens.push_back(s.substr(0, s.find("=")));
            tokens.push_back(s.substr(s.find("=") + 1));
        }
        else
        {
            tokens.push_back(s);
        }
    }

    /* return vector of parsed tokens */
    return tokens;
}

/* parse command line options */
void CMDparser::parseOptions(TokenList& tokens, Arguments& args)
{
    /* list of all available options */
    TokenList options = {"--inject", "--save", "--load", "--ar", "--read",
        "--config-file", "--iso-file", "--output", "--save-temps",
        "--game-id", "--help", "--usage", "--version"};

    /* check for each option */
    for (auto it = options.begin(); it != options.end(); ++it)
    {
        if (CMDparser::optionExists(tokens, *it))
        {
            /* get arg following option, store it in Arguments struct */
            std::string arg = CMDparser::getArg(tokens, *it);
            args.cmdOptions.insert(std::make_pair(*it, arg));
        }
    }
}

/* parse libraries and include directories */
void CMDparser::parseLibsAndIncludes(TokenList& tokens, Arguments& args)
{
    /* parse libraries */
    while (CMDparser::optionExists(tokens, "--lib"))
    {
        args.libs.push_back(CMDparser::getArg(tokens, "--lib"));
    }

    /* parse include directories */
    while (CMDparser::optionExists(tokens, "--include"))
    {
        args.includePaths.push_back(CMDparser::getArg(tokens, "--include"));
    }
}

/* check if user passed in option */
bool CMDparser::optionExists(TokenList& tokens, std::string option)
{
    /* check if option is in token list */
    return std::find(tokens.begin(), tokens.end(), option)
        != tokens.end();
}

/* get argument associated with option */
std::string CMDparser::getArg(TokenList& tokens, std::string option)
{
    /* find position of option flag */
    auto it = std::find(tokens.begin(), tokens.end(), option);

    /* look for arg if option exists */
    if (optionExists(tokens, option))
    { 
        /* check if token after option is valid arg */
        if (++it != tokens.end() && (*it).find("-") == std::string::npos)
        {
            /* return arg, erase option & arg from token list */
            std::string ret = *it;
            tokens.erase(it - 1, it + 1);
            return ret;
        }
        else
        {
            /* just erase option if there is no arg */
            tokens.erase(it - 1);   
        }

    }

    /* return empty string if no option found */
    return "";
}

