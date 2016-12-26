#include <iostream>

#include "CommandLineParser.h"
#include "GCI.h"
#include "Description.h"

/* check if argument is present, throw error otherwise */
void Require(std::string arg, std::string message) {

    if (arg.empty()) {

        throw std::invalid_argument(message);

    }

}

int main(int argc, char** argv)
{
try
{
    /* parse command line arguments */
    Arguments args;
    CMDparser::parse(argc, argv, args);    

    /* create library */
    if (args.ar)   
    {
        /* check for required arguments */
        Require(args.outputName, "missing library name, use --output");
        Require(args.mainArg, "missing source code directory, --ar"
            "<directory>");

        /* call function to create library */
        GCI::CreateLibrary(args.outputName, args.mainArg);
    }
    /* get 32-bit value stored at RAM address */
    else if (args.read) 
    {
        /* check for required arguments */
        Require(args.mainArg, "missing address, --read <address>");
        Require(args.isoPath, "missing iso file, use --iso-file");
    
        /* display value at address given */
        std::cout << GCI::ReadAddr(args.isoPath, args.mainArg) << std::endl;
    }
    /* compile, link, allocate code and inject to ISO  */
    else if (args.inject)
    {
        /* check for required arguments */
        Require(args.mainArg, "missing source code directory, --inject"
            "<directory>");
        Require(args.memFile, "missing memory configuration file, use"
            "--mem-config");
        Require(args.isoPath, "missing iso_file, use --iso-file");

        /* create object to handle memory config */
        MemoryConfig memConfig (args.memFile);

        /* call function to create new iso */
        GCI::CreateISO(args.isoPath, args.mainArg, memConfig, args.libs,
            args.includePaths, args.saveTemps);
    }
    /* save state of iso */
    else if (args.save)
    {
        /* check for required arguments */
        Require(args.memFile, "missing memory configuration file, use"
            "--mem-config");
        Require(args.isoPath, "missing iso_file, use --iso-file");
        Require(args.mainArg, "missing save file name, --save <file>");

        /* create object to handle memory config */
        MemoryConfig memConfig (args.memFile);

        /* call function to save iso state */
        GCI::SaveISO(args.isoPath, memConfig, args.mainArg);
    }
    /* load iso state */
    else if (args.load)
    {
        /* check for required arguments */
        Require(args.isoPath, "missing iso_file, use --iso-file");
        Require(args.mainArg, "missing save file name, --load <file>");

        /* call function to load iso state */
        GCI::LoadISO(args.isoPath, args.mainArg);
    }
}
catch (std::exception& e)
{
    std::cout << e.what() << std::endl;
    Description::displayUsage();
}
    return 0;
}

    
    

