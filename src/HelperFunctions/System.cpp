#include "HelperFunctions.h"

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
#include <algorithm>

/**** windows implementation of functions ****/

#ifdef GCI_WINDOWS

/* return list of all files in a directory with given extension */
FileList System::getFiles(std::string dir, std::string ext)
{
    /* list of files to return */
    FileList files;

    /* structs to store return values from file search (see windows.h) */
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    /* only search for files with correct extension */
    std::string search = dir + "\\*." + ext; 

    /* check if path exists */
    if ((hFind = FindFirstFile(search.c_str(), &fdFile))
        == INVALID_HANDLE_VALUE)
    {
        /* throw error if path does not exist */
        throw std::invalid_argument(dir + " directory not found");
    }

    /* if dir exists add all matching files to return vector */
    do
    {               
        /* add file name with dir appended to the front */
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
FileList System::getFiles(std::string dir, std::string ext)
{
    /* list of files to return */
    FileList files;

    /* list files with ls */
    std::string cmd = "ls " + dir + "/*." + ext + " > files.txt";
    System::runCMD(cmd);

    /* parse output of ls */
    std::ifstream filesText ("files.txt");
    std::string file;

    /* store each file name */
    while (filesText >> file)
    {
        /* add file name to list */
        files.push_back(file);
    }

    /* remove temp file */
    System::runCMD("rm files.txt");
    
    /* return file list */
    return files;
}

#endif

/* runs cmd and waits for it to finish */
int System::runCMD(std::string cmd, bool display)
{
    #ifdef GCI_WINDOWS
        std::replace(cmd.begin(), cmd.end(), '/', '\\');
    #endif

    FILE* cmd_ex = popen(cmd.c_str(), "r");
    pclose(cmd_ex);

    if (display)
    {
        std::cout << cmd << std::endl;
    }
}







