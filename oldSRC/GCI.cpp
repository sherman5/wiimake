#include "GCI.h"

#include <exception>
#include <sstream>

#include "SystemOperations.h"

/* inject code into iso */
void GCI::CreateISO(ISOhandler iso,
                   MemoryConfig mem_config,
                   CodeAssembler code_asmblr,
                   bool save_temps) {

    /* inject the code */
    iso.InjectCode(code_asmblr.GetRawASM());

    /* remove temp files */
    if (!save_temps) { code_asmblr.CleanDirectory();}

    /* overwritten instruction get replaced at 0x04 offset
       from the first memory region. The beginning of the first
       memory region is where the return call to the game is done */
    uint32_t replace_addr = (*mem_config.begin()).first + 0x04;

    /* overwrite injection command */
    iso.IsoWrite(replace_addr, mem_config.GetInjectInstruction());

}

/* save iso state, only save addresses in regions contained in mem_config */
void GCI::SaveISO(ISOhandler iso, MemoryConfig mem_config,
                  std::string file_name) {

    /* create save file */
    iso.CreateRestorePoint(mem_config, file_name);

}

/* load iso state from file */
void GCI::LoadISO(ISOhandler iso, std::string file_name) {

    /* load state */
    iso.Restore(file_name);

}

/* read address (32-bit unsigned int) from iso */
std::string GCI::ReadAddr(ISOhandler iso, uint32_t addr) {

    /* read from address */
    //TODO: throw error if not 32-bit address
    uint32_t val = iso.IsoRead(addr);

    /* convert output to string */
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();

}

/* read address (string version of 32-bit unsigned int) from iso */
std::string GCI::ReadAddr(ISOhandler iso, std::string addr) {

    /* read from address, make conversion to uint32_t */
    //TODO: throw error if not 32-bit address
    uint32_t val = iso.IsoRead(std::stoul(addr,nullptr,16));

    /* convert output to string */
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();

}

/* create static library from files in given directory */
void GCI::CreateLibrary(std::string name, std::string dir) {
    
    /* remove trailing backslash */
    if (dir.back() == '/' || dir.back() == '\\') { dir.pop_back();}

    /* create archive command */
    std::string ar_cmd = "powerpc-eabi-ar -cvr " + name + ".a";

    /* compile all source files */
    SysOp::compileFiles(dir);

    /* get object file names */
    auto objects = SysOp::getFiles(dir, "o");

    /* add object files to ar command */
    ar_cmd += " " + SysOp::concatVector(objects);

    /* rename sections in a unique way to differentiate
       between object files */
    for (unsigned int i = 0; i < objects.size(); ++i) {

        SysOp::renameSections(objects[i], std::to_string(i));

    }

    /* run archive command to create static library */
    SysOp::runCMD(ar_cmd);

}

