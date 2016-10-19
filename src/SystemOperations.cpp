#include "SystemOperations.h"

#ifdef GCI_WINDOWS

    #include <windows.h>
    #include <process.h>
    #include <stdio.h>
    
#endif

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>

#include "ObjdumpFileParser.h"

/**** windows implementation of functions ****/

#ifdef GCI_WINDOWS

/* return list of all files in a directory with given extension */
std::vector<std::string> SysOp::getFiles(std::string dir,
                                         std::string ext) {

    /* list of files to return */
    std::vector<std::string> files;

    /* structs to store return values from file search (see windows.h) */
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    /* only search for files with correct extension */
    std::string search = dir + "\\*." + ext; 

    /* check if path exists */
    if ((hFind = FindFirstFile(search.c_str(), &fdFile)) == INVALID_HANDLE_VALUE) {
        
        /* throw error if path does not exist */
        throw std::invalid_argument(dir + " directory not found");

    }

    /* if dir exists add all matching files to return vector */
    do {
                     
        // add file name with dir appended to the front */
        files.push_back(dir + '\\' + fdFile.cFileName);
    
    /* find next file */
    } while(FindNextFile(hFind, &fdFile)); 

    /* cleanup */
    FindClose(hFind);
    
    /* return list of file names */
    return files;
        
}

/**** linux implementation of functions ****/

#else

/* return list of all files in a directory with given extension */
std::vector<std::string> SysOp::getFiles(std::string dir,
                                         std::string ext) {

    /* list of files to return */
    std::vector<std::string> files;

    /* list files with ls */
    std::string cmd = "ls " + dir + "/*." + ext + " > files.txt";
    SysOp::runCMD(cmd);

    /* parse output of ls */
    std::ifstream file_list ("files.txt");
    std::string file;

    /* store each file name */
    while (file_list >> file) {

        /* add file name to list */
        files.push_back(file);

    }

    /* remove temp file */
    SysOp::runCMD("rm files.txt");
    
    /* return file list */
    return files;
        
}

#endif

/**** cross platform functions ****/

/* compiles all .c files in dir, include_paths is a
formatted string with all include paths, e.g.
"-I/path/to/include1 -I/path/to/include2" */
void SysOp::compileFiles(std::string dir, std::string include_paths) {
    
    /* get a list of all c files */
    auto sources = SysOp::getFiles(dir, "c");

    /* delete any pre-existing object file in directory */
    SysOp::runCMD(rm_cmd + " *.o");

    /* iterate through c files */
    for (auto it = sources.begin(); it != sources.end(); ++it) {

        /* create compile command */
        std::string compile_cmd = "powerpc-eabi-gcc -c " + include_paths
            + *it + " -o " + CHANGE_EXT(*it, "o");
        
        /* display and run command */
        std::cout << compile_cmd << std::endl;
        SysOp::runCMD(compile_cmd);

        //TODO: display output of compiler

     }

}

/* return a list of named (no '.') sections in the object file */
std::vector<std::string> SysOp::getNamedSections(std::string lib) {

    /* get object dump of object file */
    std::string cmd = "powerpc-eabi-objdump -D " + lib + " > objdump.txt";
    SysOp::runCMD(cmd);

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

    /* close the file parser */
    it.close();

    /* remove the text file */
    runCMD(rm_cmd + " objdump.txt");

    /* return list of section names */
    return sections;

}

/* create a string by concatenating each element of the vector,
   appending prefix before each string */
std::string SysOp::concatVector(std::vector<std::string> vec,
                                std::string prefix) {

    /* iterate through each string and add it to the return string */
    std::string ret_string;
    for (auto it = vec.begin(); it != vec.end(); ++it) {

        /* add prefix to the string first */
        ret_string += prefix + *it + " ";

    }

    return ret_string;

}

/* rename sections in object file since sections with '.' are ignored, ident makes names unique */
void SysOp::renameSections(std::string file, std::string id) {

    //TODO: make this general, not specific to this list of
    //section names

    /* create objcopy command */
    std::string cmd = "powerpc-eabi-objcopy";
    cmd += " --rename-section .text=text" + id;
    cmd += " --rename-section .rodata=rodata" + id;
    cmd += " --rename-section .sdata=sdata" + id;
    cmd += " --rename-section .data=data" + id;
    cmd += " --rename-section .gnu.attributes=attr" + id;
    cmd += " " + file;

    /* execute command */
    runCMD(cmd);

}

/* runs cmd and waits for it to finish */
int SysOp::runCMD(std::string cmd) {
    
    FILE* cmd_ex = popen(cmd.c_str(), "r");
    pclose(cmd_ex);

}







