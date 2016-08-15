#include <cstdio>
#include <cstring>
#include <iostream>
#include <argp.h>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "TextFileParser.h"


const char* argp_program_version = "GamecubeCodeInjector 0.1";
static char doc[] = "GCI early development version";
static char args_doc[] = "ISOFILE REGIONFILE";

static struct argp_option options[] = {
    {"inject", 'i', "FILE", 0, "inject code in FILE"},
    {"save", 's', "FILE", 0, "save state of ISOFILE to FILE"},
    {"load", 'l', "FILE", 0, "restore ISOFILE to state saved in FILE"},
    {0}
};

struct arguments {

    char* args[2];
    int inject, save, load;
    char *inject_file, *save_file, *load_file;

};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {

    struct arguments *arguments = (struct arguments*) state->input;

    switch (key) {

        case 'i':
            arguments->inject = 1;
            arguments->inject_file = arg;
            break;
        case 's':
            arguments->save = 1;
            arguments->save_file = arg;
            break;
        case 'l':
            arguments->load = 1;
            arguments->load_file = arg;
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

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[]) {

/*    struct arguments arguments;

    arguments.inject = 0;
    arguments.save = 0;
    arguments.load = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int third_arg = arguments.inject + arguments.save + arguments.load;

    if (third_arg != 1) {
        
        std::cout << "invalid number of arguments: must exactly one of --inject, --save, or --load" << std::endl;
        return 1;

    } else {

        ISOhandler iso (arguments.args[0]);
        std::string region_file = arguments.args[1];
    
        if (arguments.inject) {

            iso.InjectFile(arguments.inject_file);

        } else if (arguments.save) {

            iso.CreateRestorePoint(region_file, arguments.save_file);

        } else if (arguments.load) {

            iso.Restore(arguments.load_file);

        }
        
        return 0;

    }*/
    if (argc < 2) {
        
        std::cout << "too few arguments" << std::endl;
        return 1;
   
    } else {
    
        ISOhandler iso (argv[1]);
    
        std::vector<std::string> libs;
        std::string cmd = "ls InjectionCode/libs/*.a";
        FILE* file_list = popen(cmd.c_str(), "r");
        
        char file[500], *pos;    

        while (std::fgets(file, 500, file_list)) {

            if ((pos = strchr(file, '\n')) != NULL) {
            
                *pos = '\0';

            }

            libs.push_back(file);

        }

        CodeAssembler code (argv[2], argv[3], libs);
        iso.InjectCode(code.GetRawASM());
        iso.IsoWrite(0x80377998, 0x4BE19748);
        iso.IsoWrite(0x801910E0, 0x4800000D);
        iso.IsoWrite(0x801910E4, 0x7EE3BB78);
        iso.IsoWrite(0x801910E8, 0x481E68B4);

    }
  
}
