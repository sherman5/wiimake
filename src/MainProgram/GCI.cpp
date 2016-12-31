#include "GCI.h"
#include "../HelperFunctions/HelperFunctions.h"
#include "Description.h"

#include <iostream>

/* check if argument is present, throw error otherwise */
void Require(Arguments& args, std::string arg, std::string message)
{
    if ((!args.cmdOptions.count(arg) && !args.configOptions.count(arg))
        || (args.cmdOptions.count(arg) && args.cmdOptions[arg] == ""))
    {
        throw std::invalid_argument("\nerror: " + message);
    }
}

/* check that valid args are passed */
void GCI::checkArgs(Arguments& args)
{
    /* check for help/usage/version */
    if (args.cmdOptions.count("--help"))
    {
        std::cout << Description::summary << std::endl;
        std::cout << Description::help << std::endl;
        exit(0);
    }
    else if (args.cmdOptions.count("--usage"))
    {
        std::cout << Description::usage << std::endl;
        exit(0);
    }
    else if (args.cmdOptions.count("--version"))
    {
        std::cout << Description::version << std::endl;
        exit(0);
    }

    /* check if only one main option was passed */
    if (args.cmdOptions.count("--inject")
        + args.cmdOptions.count("--save")
        + args.cmdOptions.count("--load")
        + args.cmdOptions.count("--ar")
        + args.cmdOptions.count("--read") != 1)
    {
        /* throw exception for incorrect number of args */
        throw std::invalid_argument("must specify exactly one of "
            "--inject, --save, --load, --ar, --read");
    }

    /* check that enough arguments are passed */
    if (args.cmdOptions.count("--ar"))
    {
        Require(args, "--output", "need library name, use --output");
        Require(args, "--ar", "missing source directory, --ar=<directory>");
    }
    else
    {
        Require(args, "--config-file", "no --config-file given");
        Require(args, "--iso-file", "no --iso-file given");
        Require(args, "--game-id", "no --game-id given");
        Require(args, "code_start", "code_start missing in config file");
        Require(args, "code_end", "code_end missing in config file");
        Require(args, "DOL_start", "DOL_start missing in config file");

        if (args.addressTable.size() == 0)
        {
            throw std::invalid_argument("address_table missing in config"
                " file");
        }
    }

    /* check inject specific arguments */
    if (args.cmdOptions.count("--inject"))
    {
        Require(args, "--inject", "missing source directory of"
            " c code to inject, --inject=<directory>");
        Require(args, "inject_address", "inject_address missing in"
            " config file");
        Require(args, "original_instruction", "original_instruction"
            " missing in config file");
        
        if (args.memRegions.size() == 0)
        {
            throw std::invalid_argument("memory_regions missing in"
                " config file");
        }
    }    

    /* check for main argument */    
    if (args.cmdOptions.count("--save"))
    {
        Require(args, "--save", "missing save file name, --save=<file>");
    }
    if (args.cmdOptions.count("--load"))
    {
        Require(args, "--load", "missing load file name, --load=<file>");
    }
    if (args.cmdOptions.count("--read"))
    {
        Require(args, "--read", "missing read address, --read=<address>");
    }
}

/* inject code into iso */
void GCI::createISO(Arguments& args)
{
    /* get assembly code */    
    auto code = Builder::getASM(args);

    /* inject code */
    ISO::injectCode(code, args);

    /* remove temp files */
    if (!args.cmdOptions.count("--save-temps")) { Builder::cleanDirectory();}
}

/* save iso state */
void GCI::saveISO(Arguments& args)
{
    ISO::saveState(args);
}

/* load iso state from file */
void GCI::loadISO(Arguments& args)
{
    ISO::loadState(args);
}

/* read address (string version of 32-bit unsigned int) from iso */
uint32_t GCI::readAddr(Arguments& args)
{
    return ISO::read(args.cmdOptions["--read"], args);
}

/* create static library from files in given directory */
void GCI::createLibrary(Arguments& args)
{
    Builder::buildLibrary(args);
}

