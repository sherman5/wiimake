#include "Parser.h"
#include "Arguments.h"

#include <string>
#include <algorithm>
#include <iostream>

/* parse config file */
void ConfigParser::parse(Arguments& args)
{
    /* get tokens */
    TokenList tokens = ConfigParser::getTokens(args.configFile);

    /* store all variables in file */
    ConfigParser::storeAllVariables(tokens, args);

    /* check variables */
    ConfigParser::checkArgs(args);
}

/* parse single line of file, add to token list */
void ConfigParser::parseLine(std::string s, TokenList& tokens)
{
   /* separate equal signs (except in flags) */
    if (s.find("=") != std::string::npos
        && s.find("-") == std::string::npos)
    {
        /* separate equal sign from string */
        if (s.front() != '=') {tokens.push_back(s.substr(0, s.find("=")));}
        tokens.push_back("=");
        if (s.back() != '=') {tokens.push_back(s.substr(s.find("=") + 1));}
    }
    else
    {
        tokens.push_back(s);
    }
}

/* convert file to vector of strings */
TokenList ConfigParser::getTokens(std::string fileName)
{
    /* open up config file */
    std::ifstream configFile (fileName, std::ios::in);

    /* iterate through each line, separate tokens */
    std::string line;
    TokenList tokens;
    while (!configFile.eof())
    {
        /* throw away whitespace, look for comment/section start */
        configFile >> std::ws;
        if (configFile.peek() == ';' || configFile.peek() == '[')
        {
            /* ignore comment/section start */
            configFile.ignore(std::numeric_limits<int>::max(), '\n');
        }
        else if (configFile.peek() != EOF)
        {
            /* parse single line */
            configFile >> line;
            ConfigParser::parseLine(line, tokens); 
        }
    }

    /* close config file, return tokens */
    configFile.close();
    return tokens;
}

/* store all variables in file */
void ConfigParser::storeAllVariables(TokenList& tokens, Arguments& args)
{
    /* find all equals signs */
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        /* find variable with some values */
        if (*it == "=" && !(it + 1 == tokens.end()
            || (it + 2 != tokens.end() && *(it + 2) == "=")))
        {
            /* store variable name, get first value */
            std::string name = *(it - 1);
            TokenList::iterator last, first = ++it;

            /* read values until next variable */
            while (it != tokens.end() && *it != "=") { ++it;}
            if (it != tokens.end())
            {
                last = (--it)--;
            }
            else
            {
                last = it--;
            }
    
            /* store variable */            
            ConfigParser::storeVariable(args, name, TokenList(first, last));
        }
    }
}

/* store the memory regions variable */
void ConfigParser::storeMemRegions(Arguments& args, TokenList table)
{
    for (auto it = table.begin(); it != table.end(); ++it)
    {
        /* get beginning and end of region */
        std::string begin = (*it).substr(0, (*it).find("-"));
        std::string end = (*it).substr((*it).find("-") + 1);

        /* add region */
        args.memRegions.push_back(MemRegion(begin, end));
    }
    std::sort (args.memRegions.begin(), args.memRegions.end());
}

/* store the fixed symbols variable */
void ConfigParser::storeFixedSymbols(Arguments& args, TokenList symbols)
{
    for (auto it = symbols.begin(); it != symbols.end(); ++it)
    {
        std::string name = *(it);
        uint32_t addr = stoul(*(++it), nullptr, 16);
        uint32_t inst = stoul(*(++it), nullptr, 16);
        args.fixedSymbols.push_back(FixedSymbol(name, addr, inst));
    }
}

/* store a single variable */
void ConfigParser::storeVariable(Arguments& args, std::string name,
TokenList values)
{
    /* find variable name */
    TokenList variables = {"REGIONS", "SOURCES", "LIBRARIES",
        "INCLUDE_PATHS", "COMPILER_FLAGS", "LINKER_FLAGS",
        "FIXED_SYMBOLS"};
    
    auto pos = std::find(variables.begin(), variables.end(), name);

    /* find correct variable, store accordingly */
    switch (std::distance(variables.begin(), pos))
    {
        case 0:
            ConfigParser::storeMemRegions(args, values);
            break;
        case 1:
            args.sources = values;
            break;
        case 2:
            args.libs = values;
            break;
        case 3:
            args.includePaths = values;
            break;
        case 4:
            args.compileFlags = values;
            break;
        case 5:
            args.linkFlags = values;
            break;
        case 6:
            ConfigParser::storeFixedSymbols(args, values);
            break;
        default:
            ConfigParser::storeStaticOverwrite(args, name, values);
            break;
    }
}

/* store static overwrite given, default call, need to handle errors */
void ConfigParser::storeStaticOverwrite(Arguments& args, std::string name,
TokenList values)
{
    /* store address-value pair from config file */
    try 
    {
        uint32_t address = stoul(name, nullptr, 16);
        uint32_t value = stoul(values.front(), nullptr, 16);
        args.staticOverwrites.push_back(std::make_pair(address, value));
    }
    catch(std::exception& e)
    {
        throw std::invalid_argument("invalid address/value for static"
            " overwrite given in config file: " + name);
    }

    /* check that only one value was given */
    INVALID_ARG(values.size() > 1, "too many values in config file"
        " for static overwrite: " + name);
}

/* verify correct arguments were given in config file */
void ConfigParser::checkArgs(Arguments& args)
{
    INVALID_ARG(args.fixedSymbols.empty(), "missing entry point");
    INVALID_ARG(args.sources.empty(), "no SOURCES (config file)");
}
