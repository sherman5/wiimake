#include <cstdio>
#include <cstring>
#include <iostream>
#include <argp.h>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "TextFileParser.h"

const char* argp_program_version = "GamecubeCodeInjector v0.2";
static char doc[] = "GCI working CLI for linux. Only basic features implemented";
static char args_doc[] = "ISOFILE REGIONFILE";

static struct argp_option options[] = {
    {"inject", 'i', "DIR", 0, "compile and inject code in DIR"},
    {"save", 's', "FILE", 0, "save state of ISOFILE to FILE"},
    {"load", 'l', "FILE", 0, "restore ISOFILE to state saved in FILE"},
    {0}
};

struct arguments {

    char* args[2];
    int inject, save, load;
    char *inject_dir, *save_file, *load_file;

};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {

    struct arguments *arguments = (struct arguments*) state->input;

    switch (key) {

        case 'i':
            arguments->inject = 1;
            arguments->inject_dir = arg;
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

    struct arguments arguments;

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

            //TODO: allow user to pass directory containing libraries
            std::vector<std::string> libs;
            std::string dir = std::string(arguments.inject_dir);
            if (dir.back() == '/') {dir.pop_back();}
            std::string cmd = "ls " + dir + "/libs/*.a";
            FILE* file_list = popen(cmd.c_str(), "r");
            
            char file[500], *pos;    

            while (std::fgets(file, 500, file_list)) {

                if ((pos = strchr(file, '\n')) != NULL) {
                
                    *pos = '\0';

                }

                libs.push_back(file);

            }
        
            pclose(file_list);

            CodeAssembler code (arguments.inject_dir, region_file, libs);

            iso.InjectCode(code.GetRawASM());

            code.CleanDirectory();

        } else if (arguments.save) {

            iso.CreateRestorePoint(region_file, arguments.save_file);

        } else if (arguments.load) {

            iso.Restore(arguments.load_file);

        }
        
        return 0;

    }

}
