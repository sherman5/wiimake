/* structure adapted from https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include <argp.h>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "RegionFileParser.h"

const char* argp_program_version = "GamecubeCodeInjector v0.3";
static char doc[] = "GCI working CLI for linux. All core features implemented.";
static char args_doc[] = "ISOFILE REGIONFILE";

static struct argp_option options[] = {
    {"inject", 1, "DIR", 0, "compile and inject code in DIR"},
    {"save", 2, "FILE", 0, "save state of ISOFILE to FILE"},
    {"load", 3, "FILE", 0, "restore ISOFILE to state saved in FILE"},
    {"lib", 'l', "DIR", 0, "link against libraries in DIR"},
    {"include", 'I', "DIR", 0, "search for header files in DIR"},
    {0}
};

struct arguments {

    std::string args[2];
    int inject, save, load;
    std::string inject_dir, save_file, load_file;
    std::vector<std::string> libs, include_dirs;

};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {

    struct arguments *arguments = (struct arguments*) state->input;

    switch (key) {

        case 1:
            arguments->inject = 1;
            arguments->inject_dir = arg;
            break;
        case 2:
            arguments->save = 1;
            arguments->save_file = arg;
            break;
        case 3:
            arguments->load = 1;
            arguments->load_file = arg;
            break;
        case 'l':
            arguments->libs.push_back(arg);
            break;
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

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[]) {

    struct arguments arguments;

    arguments.inject = 0;
    arguments.save = 0;
    arguments.load = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int third_arg = arguments.inject + arguments.save + arguments.load;

    if (third_arg != 1) {
        
        std::cout << "invalid number of arguments: must have exactly one of --inject, --save, or --load" << std::endl;
        return 1;

    } else {

        ISOhandler iso (arguments.args[0]);
        std::string region_file = arguments.args[1];

        if (arguments.inject) {

            CodeAssembler code (arguments.inject_dir,
                                region_file,
                                arguments.include_dirs,
                                arguments.libs);

            iso.InjectCode(code.GetRawASM());
            code.CleanDirectory();

            RegionFileParser parser (region_file);
            uint32_t replace_addr = (*parser.begin()).first + 0x04;
            iso.IsoWrite(replace_addr, parser.GetInjectionInstruction());

        } else if (arguments.save) {

            iso.CreateRestorePoint(region_file, arguments.save_file);

        } else if (arguments.load) {

            iso.Restore(arguments.load_file);

        }
        
        return 0;

    }

}
