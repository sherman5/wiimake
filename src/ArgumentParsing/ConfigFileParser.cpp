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
void ConfigParser::parseLine(std::string line, TokenList& tokens)
{
   /* separate equal signs */
    if (line.find("=") != std::string::npos)
    {
        /* add string before equal sign */
        if (line.front() != '=')
        {
            tokens.push_back(line.substr(0, line.find("=")));
        }

        /* add equal sign */
        tokens.push_back("=");

        /* add string after equal sign */
        if (line.back() != '=')
        {
            tokens.push_back(line.substr(line.find("=") + 1));
        }
    }
    else
    {
        tokens.push_back(line);
    }
}

/* convert file to vector of strings */
TokenList ConfigParser::getTokens(std::string fileName)
{
    /* open up config file */
    std::ifstream configFile (fileName, std::ios::in);

    /* list of tokens to return */
    TokenList tokens;

    /* iterate through each line */
    std::string line;
    while (!configFile.eof())
    {
        /* throw away whitespace, look for comment/section start */
        configFile >> std::ws;
        if (configFile.peek() == ';' || configFile.peek() == '[')
        {
            /* ignore comment */
            configFile.ignore(std::numeric_limits<int>::max(), '\n');
        }
        else if (configFile.peek() != EOF)
        {
            /* parse single line */
            configFile >> line;
            ConfigParser::parseLine(line, tokens); 
        }
    }

    /* close config file */
    configFile.close();

    /* return vector of tokens */
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
}

/* store a single variable */
void ConfigParser::storeVariable(Arguments& args, std::string name,
TokenList values)
{
    /* find variable name */
    TokenList variables = {"REGIONS", "SOURCES", "LIBRARIES",
        "INCLUDE_PATHS", "COMPILER_FLAGS", "LINKER_FLAGS",
        "ADDRESS", "INSTRUCTION", "ENTRY"};
    
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
            args.injectAddress = stoul(values[0], nullptr, 16);
            if (values.size() == 1) {break;}
        case 7:
            args.originalInstruction = stoul(values[0], nullptr, 16);
            if (values.size() == 1) {break;}
        case 8:
            args.entry = values[0];
            if (values.size() == 1) {break;}
        case ' ':
            throw std::invalid_argument("too many values in config file"
                " for variable " + name);
            break;
        default:
            throw std::invalid_argument("unrecognized variable in"
                " config file: " + name);
            break;
    }
}

/* verify corect arguments were given in config file */
void ConfigParser::checkArgs(Arguments& args)
{
    if (args.injectAddress == 0)
    {
        throw std::invalid_argument("missing inject address (config file)");
    }

    if (args.originalInstruction == 0)
    {
        throw std::invalid_argument("missing instruction (config file)");
    }

    if (args.entry == "")
    {
        throw std::invalid_argument("missing entry point (config file)");
    }   

    if (args.sources.empty())
    {
        throw std::invalid_argument("no source files given");
    }
}

