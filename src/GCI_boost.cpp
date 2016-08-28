#include <string>
#include <iostream>
#include <exception>
#include <stdint.h>
#include <boost/program_options.hpp>

#include "ISOhandler.h"
#include "CodeAssembler.h"
#include "RegionFileParser.h"
#include "ProgramInfo.h"

namespace po = boost::program_options;

const std::string usage_message = "Usage: GCI [-IDIR, --include=DIR] [-lDIR, --lib=DIR] [--inject=DIR] [--load=FILE] [--save=FILE] [--save-temps] [-v, --verbose] [-H, --help] [-V, --version] [--usage] ISOFILE REGIONFILE";

int main(int argc, char* argv[]) {

    try {

        /* required arguments */
        po::positional_options_description pos_opt;
        pos_opt.add("iso_file", 1);
        pos_opt.add("region_file", 1);

        /* program options */
        po::options_description desc ("Allowed options");

        desc.add_options()
            ("help,H", "display help message")
            ("version,V", "display version information")
            ("usage", "show program syntax")
            ("inject", po::value<std::string>(), "inject code in directory (arg = dir path)")
            ("include,I", po::value< std::vector<std::string> >(), "include path")
            ("lib,l", po::value< std::vector<std::string> >(), "link against library") 
            ("save", po::value<std::string>(), "save iso state to file (arg = file name)")
            ("load", po::value<std::string>(), "load iso state from file (arg = file name)")
            ("verbose,v", po::value<bool>(), "verbose output")
            ("save-temps", po::value<bool>(), "don't delete temporary files created by build process")
        ;

        /* parse command line */
        po::variables_map vm;
        po::store (po::command_line_parser(argc, argv).options(desc).positional(pos_opt).run(), vm);

        if (vm.count("help")) {
            
            std::cout << usage_message << std::endl;        
            std::cout << program_doc << std::endl;
            std::cout << desc << std::endl;
            return 0;

        }

        if (!vm.count("iso_file") || !vm.count("region_file")) {

            throw std::invalid_argument("missing iso_file and/or region_file arguments");
        
        }

        po::notify(vm);

        /* get iso file */
        ISOhandler iso (vm["iso_file"].as<std::string>());
        
        if (vm.count("version")) {

            std::cout << program_version << std::endl;

        } else if (vm.count("usage")) {

            std::cout << usage_message << std::endl;

        } else if (vm.count("inject") + vm.count("save") + vm.count("load") != 1) {

            throw std::invalid_argument("need to specify exactly one of --inject, --save, --load");

        } else if (vm.count("save")) {

            iso.CreateRestorePoint(vm["region_file"].as<std::string>(), vm["save"].as<std::string>());

        } else if (vm.count("load")) {

            iso.Restore(vm["load"].as<std::string>());

        } else if (vm.count("inject")) {

            if (vm.count("verbose")) {}
    
            /* create object to handle compiling, allocation, and linking */
            CodeAssembler code (vm["inject"].as<std::string>(),
                                vm["region_file"].as<std::string>(),
                                vm["include"].as< std::vector<std::string> >(),
                                vm["lib"].as< std::vector<std::string> >());

            /* inject the code */
            iso.InjectCode(code.GetRawASM());

            /* remove temp files */
            if (!vm.count("save-temps")) {
                
                code.CleanDirectory();

            }

            /* overwrite injection command */
            RegionFileParser parser (vm["region_file"].as<std::string>());
            uint32_t replace_addr = (*parser.begin()).first + 0x04;
            iso.IsoWrite(replace_addr, parser.GetInjectionInstruction());

        }


    } catch (std::exception& e) {
    
        std::cout << usage_message << std::endl;
        std::cerr << "error: " << e.what() << std::endl;

    } catch (...) {

        std::cout << usage_message << std::endl;
        std::cerr << "error: unknown exception" << std::endl;

    }

    return 0;

}