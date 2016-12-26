#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "CommandLineParser.h"
#include "Description.h"

/* check if user passed in option */
bool CMDparser::optionExists(std::vector<std::string> tokens, std::string
option)
{
    return std::find(tokens.begin(), tokens.end(), option)
        != tokens.end();
}

/* get argument associated with option */
std::string CMDparser::getArg(std::vector<std::string>& tokens, std::string
option)
{
    /* find position of option flag */
    auto it = std::find(tokens.begin(), tokens.end(), option);

    /* return argument if it exists */
    if (optionExists(tokens, option))
    { 
        if (++it != tokens.end() && (*it).find("-") == std::string::npos)
        {
            std::string ret = *it;
            tokens.erase(it - 1, it);
            return ret;
        }
        else
        {
            tokens.erase(it - 1);   
        }

    }
    return "";
}

/* process command line args and store in struct */
void CMDparser::storeArgs(std::vector<std::string> tokens, Arguments& args)
{
    /* check for help or usage option */
    if (optionExists(tokens, "--help"))
    {
        Description::displayHelp();
    }
    else if (optionExists(tokens, "--usage"))
    {
        Description::displayUsage();
    }
    else if (optionExists(tokens, "--version"))
    {
        Description::displayVersion();
    }

    /* parse main option */
    std::string s;
    if (optionExists(tokens, "--inject")) {args.inject = 1; s = "--inject";}
    if (optionExists(tokens, "--save")) {args.save = 1; s = "--save";}
    if (optionExists(tokens, "--load")) {args.load = 1; s = "--load";}
    if (optionExists(tokens, "--ar")) {args.ar = 1; s = "--ar";}
    if (optionExists(tokens, "--read")) {args.read = 1; s = "--read";}
    
    /* check that only one main option was passed */
    if (args.inject + args.save + args.load + args.ar + args.read != 1)
    {
        throw std::invalid_argument("invalid number of commands, must have"
            "exactly one of: --inject, --save, --load, --ar, --read");
    }

    /* get argument for main option */
    args.mainArg = getArg(tokens, s);

    /* process secondary options */
    args.memFile = getArg(tokens, "--mem-config");
    args.isoPath = getArg(tokens, "--iso-path");
    args.outputName = getArg(tokens, "--output");
    if (optionExists(tokens, "--save-temps"))
    {
        args.saveTemps = 1;
        getArg(tokens, "--save-temps");
    }

    /* parse libraries and include paths */
    while (optionExists(tokens, "--lib"))
    {
        args.libs.push_back(getArg(tokens, "--lib"));
    }

    while (optionExists(tokens, "--include"))
    {
        args.includePaths.push_back(getArg(tokens, "--include"));
    }

    /* check if any options left */
    if (!tokens.empty())
    {
        throw std::invalid_argument("unrecognized option: "
            + tokens.front());
    }
}

void CMDparser::parse(int argc, char** argv, Arguments& args)
{
    std::vector<std::string> tokens;    
    parse(argc, argv, args);
    for (int i = 1; i < argc; ++i)
    {
        std::string s (argv[i]);
        if (s.find("=") != std::string::npos)
        {
            tokens.push_back(s.substr(1, s.find("=")));
            tokens.push_back(s.substr(s.find("=") + 1));
        }
        else
        {
            tokens.push_back(s);
        }
    }
    storeArgs(tokens, args);
}

