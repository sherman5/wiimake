#include <sstream>
#include <string> 

#include "GCI.h"
#include "../HelperFunctions/HelperFunctions.h"

/* inject code into iso */
void GCI::CreateISO(Arguments& args)
{
    /* get assembly code */    
    auto code = Builder::getASM(args);

    /* inject code */
    ISO::injectCode(code, args);

    /* remove temp files */
    if (!args.cmdOptions.count("--save-temps")) { Builder::cleanDirectory();}
}

/* save iso state */
void GCI::SaveISO(Arguments& args)
{
    /* create save file */
    ISO::saveState(args);
}

/* load iso state from file */
void GCI::LoadISO(Arguments& args)
{
    /* load state */
    ISO::loadState(args);
}

/* read address (string version of 32-bit unsigned int) from iso */
std::string GCI::ReadAddr(Arguments& args)
{
    /* read from address, make conversion to uint32_t */
    //TODO: throw error if not 32-bit address
    uint32_t val = ISO::read(args.cmdOptions["--read"], args);

    /* convert output to string */
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();
}

/* create static library from files in given directory */
void GCI::CreateLibrary(Arguments& args)
{
    /* compile source files, get object names */
    auto objects = Builder::getObjectFiles(args.cmdOptions["--ar"]);

    /* create archive command */
    std::string cmd = "powerpc-eabi-ar -cvr " + args.cmdOptions["--output"];

    /* add object files to command */
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        cmd += " " + *it;
    }

    /* rename sections in a unique way to differentiate
       between object files */
    for (unsigned int i = 0; i < objects.size(); ++i)
    {
        ObjectFile::renameSections(objects[i], std::to_string(i));
    }

    /* run archive command to create static library */
    System::runCMD(cmd);
}

