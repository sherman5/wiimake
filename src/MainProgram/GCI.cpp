#include <sstream>
#include <string> 

#include "GCI.h"
#include "../HelperFunctions/HelperFunctions.h"

/* inject code into iso */
void GCI::CreateISO(std::string isoPath,
                    std::string sourceDir,
                    MemoryConfig& memConfig,
                    FileList& libs,
                    FileList& includeDirs,
                    bool saveTemps)
{
    /* get assembly code */    
    auto code = Builder::getASM(sourceDir, memConfig, libs, includeDirs);

    /* inject code */
    ISO::injectCode(isoPath, code);

    /* remove temp files */
    if (!saveTemps) { Builder::cleanDirectory();}

}

/* save iso state, only save addresses in regions contained in mem_config */
void GCI::SaveISO(std::string isoPath, MemoryConfig& memConfig,
                  std::string fileName) {

    /* create save file */
    ISO::saveState(isoPath, memConfig, fileName);

}

/* load iso state from file */
void GCI::LoadISO(std::string isoPath, std::string fileName) {

    /* load state */
    ISO::loadState(isoPath, fileName);

}

/* read address (string version of 32-bit unsigned int) from iso */
std::string GCI::ReadAddr(std::string isoPath, std::string addr) {

    /* read from address, make conversion to uint32_t */
    //TODO: throw error if not 32-bit address
    uint32_t val = ISO::read(isoPath, std::stoul(addr, nullptr, 16));

    /* convert output to string */
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();

}

/* create static library from files in given directory */
void GCI::CreateLibrary(std::string name, std::string dir) {
    
    /* compile source files, get object names */
    auto objects = Builder::getObjectFiles(dir, FileList(), FileList());

    /* create archive command */
    std::string cmd = "powerpc-eabi-ar -cvr " + name + ".a";

    /* add object files to command */
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        cmd += " " + *it;
    }

    /* rename sections in a unique way to differentiate
       between object files */
    for (unsigned int i = 0; i < objects.size(); ++i) {

        ObjectFile::renameSections(objects[i], std::to_string(i));

    }

    /* run archive command to create static library */
    System::runCMD(cmd);

}

