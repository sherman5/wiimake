#include <iostream>

#include "Parser.h"
#include "GCI.h"
#include "Description.h"

/* run the program given command line arguments */
void RunProgram(Arguments& args)
{
    /* create library */
    if (args.cmdOptions.count("--ar"))   
    {
        /* call function to create library */
        GCI::createLibrary(args);
    }
    /* get 32-bit value stored at RAM address */
    else if (args.cmdOptions.count("--read")) 
    {
        /* display value at address given */
        std::cout << std::hex << GCI::readAddr(args) << std::endl;
    }
    /* compile, link, allocate code and inject to ISO  */
    else if (args.cmdOptions.count("--inject"))
    {
        /* compile, allocate, link, and inject code */
        GCI::createISO(args);
    }
    /* save state of iso */
    else if (args.cmdOptions.count("--save"))
    {
        /* call function to save iso state */
        GCI::saveISO(args);
    }
    /* load iso state */
    else if (args.cmdOptions.count("--load"))
    {
        /* call function to load iso state */
        GCI::loadISO(args);
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
        GCI::checkArgs(args);
        RunProgram(args);
    }
    /* error in program */
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << Description::usage << std::endl;
    }
}

   

