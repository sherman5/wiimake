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
    std::string cmd = "ls " + dir + "/*." + ext;
    FILE* file_list = popen(cmd.c_str(), "r");

    /* store each string as a file name */
    char file[256], *pos;    

    while (std::fgets(file, 256, file_list)) {

        /* delete newline character at end of string */
        if ((pos = strchr(file, '\n')) != NULL) {
        
            *pos = '\0';

        }

        files.push_back(file);

    }

    /* close the ls command */
    pclose(file_list);

    return files;
        
}

/* create a text file from a file stream */
void create_text_file(FILE* in_file, std::string name) {

    /* open up text file */
    std::ofstream text_file (name);

    /* write every character until end-of-file */
    char c;
    while ((c = std::getc(in_file)) != EOF) {

        text_file << c;

    }
    text_file.close();

}

/* return a list of named (no '.') sections in the object file */
std::vector<std::string> get_named_sections(std::string lib) {

    /* get object dump of object file */
    std::string cmd = "powerpc-eabi-objdump -D " + lib;
    FILE* objdump = popen(cmd.c_str(), "r");
    
    /* store object dump in text file and close command */
    create_text_file(objdump, "objdump.txt");
    pclose(objdump);

    /* parse the text file */
    ObjdumpFileParser parser ("objdump.txt");
    ObjdumpFileParser::iterator it = parser.begin();

    /* list of sections to return */
    std::vector<std::string> sections;
    
    /* need the first section to have a comparison */
    sections.push_back(it.getSection());

    /* record the names of each section */
    for (; !it.atEnd(); ++it) {

        if (it.getSection().find(".") == std::string::npos
            && it.getSection() != sections.back()) { //new section, no '.'
       
            sections.push_back(it.getSection());

        }

    }

    /* delete text file */
    FILE* rm = popen("rm objdump.txt", "r");
    pclose(rm);

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
    FILE* obj_cpy = popen(cmd.c_str(), "r");
    pclose(obj_cpy);

}

