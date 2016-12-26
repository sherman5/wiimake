#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <stdint.h>
#include <boost/program_options.hpp>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "MemoryConfig.h"
#include "ProgramInfo.h"

/* declare namespace alias */
namespace po = boost::program_options;

/* store --usage message */
const std::string usage_message = "Usage: GCI [--mem-config <file>] [--iso-file <file>] [--save <file>] [--load <file>] [--inject <directory>] [-l<library>, --lib <library>] [-I<directory>, --include <directory>] [--save-temps] [--ar <directory>] [-o <output>, --output <output>] [--read <address>] [-H, --help] [-V, --version] [--usage]";

/* check if argument is present, throw error otherwise */
void Require(std::string arg, std::string message) {

    /* nothing stored in string */
    if (arg.empty()) {

        /* throw error */
        throw::invalid_argument(message);

    }

}

/* run CLI */
int main(int argc, char* argv[]) {

    /* incorrect command line arguments result in a thrown error */
    try {

         /* program options */
        po::options_description desc ("Allowed options");

        /* makes next code cleaner */
        typedef po::value<std::string>() string_arg;

        /* add program options */
        desc.add_options()
            ("help,H", "display help message")
            ("version,V", "display version information")
            ("usage", "show program syntax")
            ("mem-config", string_arg, "specify a memory configuration file)
            ("iso-file", string_arg, "specify which iso file to use")
            ("save", string_arg, "save state of iso in --iso-file to <arg>")
            ("load", string_arg, "restore iso in --iso-file to state saved in <arg>")
            ("inject", string_arg, "compile C code in <arg> and inject into iso specified by --iso-file, only overwriting memory regions specified by --mem-config")
            ("lib,l", po::value< std::vector<std::string> >(), "link against <arg>") 
            ("include,I", po::value< std::vector<std::string> >(), "search for header files in <arg>")
            ("save-temps", "don't delete temporary files created by injection process")
            ("ar", string_arg, "compile C code in <arg> and create static library")
            ("output", string_arg, "specify library name to be <arg>")
            ("read", string_arg, "read memory at <arg> in iso file (from --iso-file)")
        ;

        /* parse command line */
        po::variables_map vm;
        po::store (po::command_line_parser(argc, argv).options(desc).run(), vm);

        /* meta information options */
        if (vm.count("help")) {
            
            std::cout << "\n" << usage_message << std::endl;        
            std::cout << "\n" << program_doc << std::endl;
            std::cout << "\n" << desc << std::endl;
            return 0;

        } else if (vm.count("version")) {

            std::cout << program_version << std::endl;
            return 0;

        } else if (vm.count("usage")) {

            std::cout << "\n" << usage_message << std::endl;
            return 0;

        }

        /* ??? taken from example */
        po::notify(vm);

        /* make sure only one command is provided */
        if (vm.count("inject") + vm.count("save") + vm.count("load") + vm.count("ar") + vm.count("read") != 1) {

            /* throw error if number of arguments is different than 1 */
            throw std::invalid_argument("invalid number of commands, must have exactly one of: --inject, --save, --load, --ar, --read");

        } else {

            /* check if memory configuration file is given */
            if (vm.count("mem-config") {

                /* create object to store configuration */
                MemoryConfig mem_config (vm["mem-config"].as<std::string>());

            }

            /* check if iso file is given */    
            if (vm.count("iso-file") {

                /* create iso handler for given iso file */
                ISOhandler iso_handler (vm["iso-file"].as<std::string>());

            }

            /* command: --ar */
            if (vm.count("ar") {

                /* check for required arguments */
                Require(vm["output"], "missing library name, use --output");
                Require(vm["ar"], "missing source code directory, --ar <directory>");

                /* call function to create library */
                GCI::CreateLibrary(vm["output"], vm["ar"]);

            /* command: --read */
            } else if (vm.count("read")) {

                /* check for required arguments */
                Require(vm["read"], "missing address, --read <address>");
                Require(vm["iso-file"], "missing iso file, use --iso-file");
            
                /* display value at address given */
                std::cout << GCI::ReadAddr(iso_handler, vm["read"]) << std::endl;

            /* command: --inject */
            } else if (vm.count("inject")) {

                /* check for required arguments */
                Require(vm["inject"], "missing source code directory, --inject <directory>");
                Require(vm["mem-config"], "missing memory configuration file, use --mem-config");
                Require(vm["iso-file"], "missing iso_file, use --iso-file");

                std::vector<std::string> include_paths, libs;

                if (vm.count("include")) { include_paths = vm["include"].as< std::vector<std::string> >();}
                if (vm.count("lib")) { libs = vm["lib"].as< std::vector<std::string> >();}


                /* create class to handle compiling, allocation, and linking */
                CodeAssembler code (vm["inject"],
                                    mem_config,
                                    include_paths,
                                    libs);

                /* call function to create new iso */
                CreateISO(iso_handler, mem_config, code, vm.count("save-temps"));

            /* command: --save */
            } else if (vm.count("save")) {

                /* check for required arguments */
                Require(vm["mem-config"], "missing memory configuration file, use --mem-config");
                Require(vm["iso-file"], "missing iso_file, use --iso-file");
                Require(vm["save"], "missing save file name, --save <file>");

                /* call function to save iso state */
                SaveISO(iso_handler, mem_config, vm["save"]);

            /* command: --load */
            } else if (vm.count("load")) {

                /* check for required arguments */
                Require(vm["iso-file"], "missing iso_file, use --iso-file");
                Require(vm["load"], "missing save file name, --load <file>");

                /* call function to load iso state */
                LoadISO(iso_handler, vm["load"]);

            }

















            if (vm.count("save")) {

                iso.CreateRestorePoint(mem_config, vm["save"].as<std::string>());

            } else if (vm.count("load")) {

            iso.Restore(vm["load"].as<std::string>());

        } else if (vm.count("inject")) {

            if (vm.count("verbose")) {}
    
            
            /* create object to handle compiling, allocation, and linking */
            CodeAssembler code (vm["inject"].as<std::string>(),
                                mem_config,
                                include_paths,
                                libs);
                                

            /* inject the code */
            iso.InjectCode(code.GetRawASM());
            
            /* remove temp files */
            if (!vm.count("save-temps")) {
                
                code.CleanDirectory();

            }

            /* overwrite injection command */
            uint32_t replace_addr = (*mem_config.begin()).first + 0x04;
            iso.IsoWrite(replace_addr, mem_config.GetInjectInstruction());

        }


    } catch (std::exception& e) {
    
        std::cout << "\n" << usage_message << std::endl;
        std::cerr << "\nerror: " << e.what() << std::endl;

    } catch (...) {

        std::cout << "\n" << usage_message << std::endl;
        std::cerr << "\nerror: unknown exception" << std::endl;

    }

    return 0;

}
