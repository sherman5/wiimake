/* structure adapted from https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <argp.h>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "MemoryConfig.h"
#include "ProgramInfo.h"
#include "GCI.h"

/* get program version from ProgramInfo.h */
const char* argp_program_version = program_version;

/* command line options */
static struct argp_option options[] = {
    {"mem-config", 1, "<file>", 0, "specify a memory configuration file"},
    {"iso-file", 2, "<file>", 0, "specify which iso file to use"},
    {"save", 3, "<file>", 0, "save state of iso in --iso-file to <file>"},
    {"load", 4, "<file>", 0, "restore iso in --iso-file to state saved in <file>"},
    {"inject", 5, "<directory>", 0, "compile C code in <directory> and inject into iso specified by --iso-file, only overwriting memory regions specified by --mem-config"},
    {"lib", 'l', "<library>", 0, "link against <library>"},
    {"include", 'I', "<directory>", 0, "search for header files in <directory>"},
    {"save-temps", 6, 0, 0, "don't delete temporary files created by injection process"},
    {"ar", 7, "<directory>", 0, "compile C code in <directory> and create static library"},
    {"output", 'o', "<output>", 0, "specify library name to be <output>"},
    {"read", 8, "<address>", 0, "read memory at <address> in iso file specified with --iso-file"},
    {0}
};

/* user provided arguments are stored here */
struct arguments {
    
    /* T/F values if flag is present */
    int inject, save, load, ar, read, save_temps;
    
    /* source directory and file names */
    std::string directory, iso_file, config_file, file_name, address;

    /* libraries and include files */
    std::vector<std::string> libs, include_dirs;

};

/* function to parse command line arguments */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {

    struct arguments *arguments = (struct arguments*) state->input;

    /* switch on current flag */
    switch (key) {

        /* --mem-config */
        case 1:
            arguments->config_file = arg;
            break;
        /* --iso-file */
        case 2:
            arguments->iso_file = arg;
            break;
        /* --save */
        case 3:
            arguments->save = 1;
            arguments->file_name = arg;
            break;
        /* --load */
        case 4:
            arguments->load = 1;
            arguments->file_name = arg;
            break;
        /* --inject */
        case 5:
            arguments->inject = 1;
            arguments->directory = arg;
            break;
        /* --save-temps */
        case 6:
            arguments->save_temps = 1;
            break;
        /* --ar */
        case 7:
            arguments->ar = 1;
            arguments->directory = arg;
            break;
        /* --read */
        case 8:
            arguments->read = 1;
            arguments->address = arg;
            break;
        /* --output */
        case 'o':
            arguments->file_name = arg;
            break;
        /* --lib */
        case 'l':
            arguments->libs.push_back(arg);
            break;
        /* --include */
        case 'I':
            arguments->include_dirs.push_back(arg);
            break;
        /* unknown option */
        default:
            return ARGP_ERR_UNKNOWN;

    }

    return 0;

}

/* struct containing all data and functions for the CLI */
static struct argp argp = { options, parse_opt, program_doc };

/* check if argument is present, throw error otherwise */
void Require(std::string arg, std::string message) {

    if (arg.empty()) {

        throw std::invalid_argument(message);

    }

}

/* run CLI */
int main(int argc, char* argv[]) {

    /* struct that contains arguments */
    struct arguments arguments;

    /* initialize to false */
    arguments.inject = 0;
    arguments.save = 0;
    arguments.load = 0;
    arguments.ar = 0;
    arguments.read = 0;
    arguments.save_temps = 0;

    /* parse all comand line arguments */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* make sure only one command is provided */
    int cmd_total = arguments.inject + arguments.save + arguments.load + arguments.ar + arguments.read;

    /* incorrect number of commands given */
    if (cmd_total != 1) {
        
        /* quit program with error */
        std::cout << "invalid number of commands, must have exactly one of: --inject, --save, --load, --ar, --read" << std::endl;
        return 1;

    /* only one command given */
    } else {

        MemoryConfig mem_config;
        ISOhandler iso_handler;

        /* check if memory configuration file is given */
        if (!arguments.config_file.empty()) {

            /* create object to store configuration */
             mem_config = MemoryConfig(arguments.config_file);

        }

        /* check if iso file is given */    
        if (!arguments.iso_file.empty()) {

            /* create iso handler for given iso file */
            iso_handler = ISOhandler(arguments.iso_file);
    
        }

        /* command: --ar */
        if (arguments.ar) {

            /* check for required arguments */
            Require(arguments.file_name, "missing library name, use --output");
            Require(arguments.directory, "missing source code directory, --ar <directory>");

            /* call function to create library */
            GCI::CreateLibrary(arguments.file_name, arguments.directory);

        /* command: --read */
        } else if (arguments.read) {

            /* check for required arguments */
            Require(arguments.address, "missing address, --read <address>");
            Require(arguments.iso_file, "missing iso file, use --iso-file");
        
            /* display value at address given */
            std::cout << GCI::ReadAddr(iso_handler, arguments.address) << std::endl;

        /* command: --inject */
        } else if (arguments.inject) {

            /* check for required arguments */
            Require(arguments.directory, "missing source code directory, --inject <directory>");
            Require(arguments.config_file, "missing memory configuration file, use --mem-config");
            Require(arguments.iso_file, "missing iso_file, use --iso-file");

            /* create class to handle compiling, allocation, and linking */
            CodeAssembler code (arguments.directory,
                                mem_config,
                                arguments.include_dirs,
                                arguments.libs);

            /* call function to create new iso */
            GCI::CreateISO(iso_handler, mem_config, code, arguments.save_temps);

        /* command: --save */
        } else if (arguments.save) {

            /* check for required arguments */
            Require(arguments.config_file, "missing memory configuration file, use --mem-config");
            Require(arguments.iso_file, "missing iso_file, use --iso-file");
            Require(arguments.file_name, "missing save file name, --save <file>");

            /* call function to save iso state */
            GCI::SaveISO(iso_handler, mem_config, arguments.file_name);

        /* command: --load */
        } else if (arguments.load) {

            /* check for required arguments */
            Require(arguments.iso_file, "missing iso_file, use --iso-file");
            Require(arguments.file_name, "missing save file name, --load <file>");

            /* call function to load iso state */
            GCI::LoadISO(iso_handler, arguments.file_name);

        }
        
        return 0;

    }

}
