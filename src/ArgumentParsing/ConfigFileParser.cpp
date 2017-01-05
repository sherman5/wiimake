#include "Parser.h"
#include "Arguments.h"

#include <string>
#include <algorithm>

/* parse config file */
void ConfigParser::parse(Arguments& args)
{
    /* get tokens */
    TokenList tokens = ConfigParser::getTokens(args);

    /* remove comments */
    ConfigParser::removeComments(tokens);

    /* store all variables in file */
    ConfigParser::storeAllVariables(tokens, args);
}

/* convert file to vector of strings */
TokenList ConfigParser::getTokens(Arguments& args)
{
    /* open up config file */
    std::ifstream configFile (args.configFile, std::ios::in);

    /* read first line */
    std::string line;
    configFile >> line;

    /* add each line to vector */
    TokenList tokens;
    while (!configFile.eof())
    {
        /* separate equal signs */
        if (line.find("=") != std::string::npos)
        {
            tokens.push_back(line.substr(0, line.find("=")));
            tokens.push_back("=");
            tokens.push_back(line.substr(line.find("=") + 1));
        }
        else
        {
            tokens.push_back(line);
        }

        /* read next line */
        configFile >> line;
    }

    /* close config file */
    configFile.close();

    /* return vector of tokens */
    return tokens;
}

/* remove all comments from config file */
void ConfigParser::removeComments(TokenList& tokens)
{
    /* iterate through all tokens */
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        /* check if beginning of comment */
        if ((*it).find(";") != std::string::npos)
        {
            /* find end of comment */
            auto commentStart = it;
            while ((*it).find("\n") == std::string::npos) { ++it;}

            /* erase comment */
            tokens.erase(commentStart, it + 1);
        }
    }
}

/* store all variables in file */
void ConfigParser::storeAllVariables(TokenList& tokens, Arguments& args)
{
    /* find all equals signs */
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if ((*it) == "=")
        {
            /* store variable name */
            std::string name = *(it - 1);
        
            /* find list of values for this variable */
            auto first = ++it;
            while ((*it) != "=") { ++it;}
            auto last = --(--it);

            /* store this variable */
            ConfigParser::storeVariable(args, name, TokenList(first, last));
        }
    }
}

/* store a single variable */
void ConfigParser::storeVariable(Arguments& args, std::string name,
TokenList values)
{
    /* find variable name */
    TokenList variables = {"REGIONS", "SOURCES", "LIBRARIES",
        "INCLUDE_PATHS", "COMPILER_FLAGS", "LINKER_FLAGS",
        "ENTRY", "ADDRESS", "INSTRUCTION"};
    
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
            args.entry = values[0];
            break;
        case 7:
            args.injectAddress = stoul(values[0], nullptr, 16);
            break;
        case 8:
            args.originalInstruction = stoul(values[0], nullptr, 16);
            break;
        default:
            throw std::invalid_argument("unrecognized variable in"
                " config file: " + name);
            break;
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

