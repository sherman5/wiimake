#include <string>
#include <vector>
#include <cstring>
#include <fstream>

#include "ObjdumpFileParser.h"
#include "FileOperations.h"

/* change extenstion of file */
std::string change_ext(std::string file, std::string ext) {

    return file.substr(0, file.find('.') + 1) + ext;

}

/* return list of all files in a directory with given extension */
std::vector<std::string> get_files(std::string dir, std::string ext) {

    /* list of files to return */
    std::vector<std::string> files;

    /* list files with ls */
    std::string cmd = "ls " + dir + "/*." + ext + " > files.txt";
    run_cmd(cmd.c_str());

    /* parse output of ls */
    std::ifstream file_list ("files.txt");
    std::string file;

    /* store each file name */
    while (file_list >> file) {

        files.push_back(file);

    }

    return files;
        
}

/* return a list of named (no '.') sections in the object file */
std::vector<std::string> get_named_sections(std::string lib) {

    /* get object dump of object file */
    std::string cmd = "powerpc-eabi-objdump -D " + lib + " > objdump.txt";
    run_cmd(cmd.c_str());

    /* parse the text file */
    ObjdumpFileParser parser ("objdump.txt");
    ObjdumpFileParser::iterator it = parser.begin();

    /* list of sections to return */
    std::vector<std::string> sections;
    
    /* need the first section to have a comparison */
    sections.push_back(it.getSection());

    /* record the names of each section */
    for (; !it.atEnd(); ++it) {

        /* new section with no '.' */
        if (it.getSection().find(".") == std::string::npos
            && it.getSection() != sections.back()) {
       
            sections.push_back(it.getSection());

        }

    }

    return sections;

}

/* create a string by concatenating each element of the vector,
   appending prefix before each string */
std::string concat_vector(std::vector<std::string> vec, std::string prefix) {

    /* iterate through each string and add it to the return string */
    std::string ret_string;
    for (auto it = vec.begin(); it != vec.end(); ++it) {

        /* add prefix to the string first */
        ret_string += prefix + *it + " ";

    }

    return ret_string;

}

/* rename sections in object file since sections with '.' are ignored, ident makes names unique */
void rename_sections(std::string file, std::string ident) {

    /* create objcopy command */
    std::string cmd = "powerpc-eabi-objcopy";
    cmd += " --rename-section .text=text" + ident;
    cmd += " --rename-section .rodata=rodata" + ident;
    cmd += " --rename-section .sdata=sdata" + ident;
    cmd += " --rename-section .data=data" + ident;
    cmd += " --rename-section .gnu.attributes=attr" + ident;
    cmd += " " + file;

    /* execute command */
    run_cmd(cmd.c_str());

}

/* runs cmd and waits for it to finish */
int run_cmd(std::string cmd) {
  
    //#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

    
    //#else

        FILE* cmd_ex = popen(cmd.c_str(), "r");
        fclose(cmd_ex);

    //#endif

}







