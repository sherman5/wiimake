#include "GCI.h"

#include <exception>
#include <sstream>

void CreateISO(ISOhandler& iso,
               MemoryConfig& mem_config,
               CodeAssembler& code_asmblr,
               bool save_temps) {

    /* inject the code */
    iso.InjectCode(code_asmblr.GetRawASM());

    /* remove temp files */
    if (!save_temps) { code_asmblr.CleanDirectory();}

    /* overwrite injection command */
    uint32_t replace_addr = (*mem_config.begin()).first + 0x04;
    iso.IsoWrite(replace_addr, mem_config.GetInjectInstruction());

}

void SaveISO(ISOhandler& iso, MemoryConfig& mem_config, std::string file_name) {

    iso.CreateRestorePoint(mem_config, file_name);

}

void LoadISO(ISOhandler& iso, std::string file_name) {

    iso.Restore(file_name);

}

std::string ReadAddr(ISOhandler& iso, uint32_t addr) {

    //TODO: throw error if not 32-bit address
    uint32_t val = iso.IsoRead(addr);
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();

}

std::string ReadAddr(ISOhandler& iso, std::string addr) {

    //TODO: throw error if not 32-bit address
    uint32_t val = iso.IsoRead(std::stoul(addr,nullptr,16));
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();

}

/* create static library from files in given directory */
void CreateLibrary(std::string name, std::string dir) {
    
    /* add all files to archive command */
    std::string ar_cmd = "powerpc-eabi-ar -cvr " + name;
    CodeAssembler code (dir, MemoryConfig(), std::vector<std::string>(), std::vector<std::string>());
    auto objects = code.CompileSourceFiles();

    for (auto it = objects.begin(); it != objects.end(); ++it) {

        ar_cmd += " " + *it;

    }

    /* run archive command to create static library */
    run_cmd(ar_cmd);

    /* rename sections in a unique way to differentiate between object files */
    for (unsigned int i = 0; i < objects.size(); ++i) {

        rename_sections(obj_files[i], std::to_string(i));

    }


}

