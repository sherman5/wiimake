/* structure adapted from https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include <argp.h>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "RegionFileParser.h"

/* current program version */
const char* argp_program_version = "GamecubeCodeInjector v0.3";

/* description of software */
static char doc[] = "GCI working CLI for linux. All core features implemented.";

/* required arguments */
static char args_doc[] = "ISOFILE REGIONFILE";

/* command line options */
static struct argp_option options[] = {
    {"inject", 1, "DIR", 0, "compile C code in DIR and inject into ISOFILE, only overwriting memory regions specified in REGIONFILE"},
    {"save", 2, "FILE", 0, "save state of ISOFILE to FILE"},
    {"load", 3, "FILE", 0, "restore ISOFILE to state saved in FILE"},
    {"lib", 'l', "DIR", 0, "link against libraries in DIR"},
    {"include", 'I', "DIR", 0, "search for header files in DIR"},
    {0}
};

/* user provided arguments are stored here */
struct arguments {

    /* iso and region file */
    std::string args[2];
    
    /* T/F values if flag is present */
    int inject, save, load;
    
    /* source directory and file names */
    std::string inject_dir, save_file, load_file;

    /* libraries and include files */
    std::vector<std::string> libs, include_dirs;

};

/* function to parse command line arguments */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {

    struct arguments *arguments = (struct arguments*) state->input;

    /* switch on current flag */
    switch (key) {

        /* --inject */
        case 1:
            arguments->inject = 1;
            arguments->inject_dir = arg;
            break;
        /* --save */
        case 2:
            arguments->save = 1;
            arguments->save_file = arg;
            break;
        /* --load */
        case 3:
            arguments->load = 1;
            arguments->load_file = arg;
            break;
        /* --lib, -l */
        case 'l':
            arguments->libs.push_back(arg);
            break;
        /* --include, -I */
        case 'I':
            arguments->include_dirs.push_back(arg);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
            /* Too many arguments. */
            argp_usage (state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 2)
            /* Not enough arguments. */
            argp_usage (state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;

}

/* struct containing all data and functions for the CLI */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[]) {

    struct arguments arguments;

    /* initialize to false */
    arguments.inject = 0;
    arguments.save = 0;
    arguments.load = 0;

    /* parse all comand line arguments */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* make sure only one argument is provided in addition to iso and region file */
    int third_arg = arguments.inject + arguments.save + arguments.load;
    if (third_arg != 1) {
        
        std::cout << "invalid number of arguments: must have exactly one of --inject, --save, or --load" << std::endl;
        return 1;

    } else {

        /* get iso file and region file */
        ISOhandler iso (arguments.args[0]);
        std::string region_file = arguments.args[1];

        /* inject code */
        if (arguments.inject) {

            /* create class to handle compiling, allocation, and linking */
            CodeAssembler code (arguments.inject_dir,
                                region_file,
                                arguments.include_dirs,
                                arguments.libs);

            /* inject the code */
            iso.InjectCode(code.GetRawASM());

            /* remove temp files */
            code.CleanDirectory();

            /* overwrite injection command */
            RegionFileParser parser (region_file);
            uint32_t replace_addr = (*parser.begin()).first + 0x04;
            iso.IsoWrite(replace_addr, parser.GetInjectionInstruction());

        /* save ISO state */
        } else if (arguments.save) {

            iso.CreateRestorePoint(region_file, arguments.save_file);

        /* load ISO state */
        } else if (arguments.load) {

            iso.Restore(arguments.load_file);

        }
        
        return 0;

    }

}
