#include <iostream>

#include "Parser.h"
#include "GCI.h"
#include "Description.h"

/* check if argument is present, throw error otherwise */
void Require(std::string arg, std::string message)
{
    if (arg.empty())
    {
        throw std::invalid_argument(message);
    }
}

/* run the program given command line arguments */
void RunProgram(Arguments& args)
{
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

    /* create library */
    if (args.cmdOptions.count("--ar"))   
    {
        /* check for required arguments */
        Require(args.cmdOptions["--output"], "missing library name,"
            " use --output");

        Require(args.cmdOptions["--ar"], "missing source code directory,"
            " --ar <directory>");

        /* call function to create library */
        GCI::CreateLibrary(args);
    }
    /* get 32-bit value stored at RAM address */
    else if (args.cmdOptions.count("--read")) 
    {
        /* check for required arguments */
        Require(args.cmdOptions["--read"], "missing address,"
            " --read <address>");

        Require(args.cmdOptions["--iso-file"], "missing iso file, use"
            " --iso-file");
    
        /* display value at address given */
        std::cout << GCI::ReadAddr(args) << std::endl;
    }
    /* compile, link, allocate code and inject to ISO  */
    else if (args.cmdOptions.count("--inject"))
    {
        /* check for required arguments */
        Require(args.cmdOptions["--inject"], "missing source"
            " code directory, --inject <directory>");

        Require(args.cmdOptions["--config-file"], "missing configuration"
            " file, use --config-file");

        Require(args.cmdOptions["--iso-file"], "missing iso file,"
            " use --iso-file");

        /* compile, allocate, link, and inject code */
        GCI::CreateISO(args);
    }
    /* save state of iso */
    else if (args.cmdOptions.count("--save"))
    {
        /* check for required arguments */
        Require(args.cmdOptions["--config-file"], "missing memory"
            " configuration file, use --mem-config");

        Require(args.cmdOptions["--iso-file"], "missing iso file,"
            " use --iso-file");

        Require(args.cmdOptions["--save"], "missing save file name,"
            " --save <file>");

        /* call function to save iso state */
        GCI::SaveISO(args);
    }
    /* load iso state */
    else if (args.cmdOptions.count("--load"))
    {
        /* check for required arguments */
        Require(args.cmdOptions["--iso-file"], "missing iso_file, use"
            " --iso-file");

        Require(args.cmdOptions["--load"], "missing save file name,"
            " --load <file>");

        /* call function to load iso state */
        GCI::LoadISO(args);
    }

}

/* CLI */
int main(int argc, const char** argv)
{
    try
    {
        /* parse command line arguments */
        Arguments args;
        CMDparser::parse(argc, argv, args);    
        ConfigParser::parse(args);

        /* run program */
        RunProgram(args);

    }
    /* user passed invalid argument */
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << Description::usage << std::endl;
    }
}

   

