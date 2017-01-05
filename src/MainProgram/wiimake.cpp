#include "Parser.h"

#include <iostream>

int main(unsigned argc, const char** argv)
{
    /* get command line arguments */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* parse meta options */
    CMDparser::parseMetaOptions(tokens);

    /* look for --save-temps option */ 
    auto pos = std::find(tokens.begin(), tokens.end(),"--save-temps");
    if (pos != tokens.end())
    {
        args.saveTemps = true;
        tokens.erase(pos);
    }    

    /* parse remaining two options */
    if (tokens.size() != 2)
    {
        std::throw invalid_argument("incorrect number of options");
        std::cout << Description::usage << std::endl;
        exit(0);
    }
    else 
    {
        /* parse file arguments */
        args.isoFile = tokens.front();
        args.configFile = tokens.back();
        ConfigParser::parse(args);

        /* inject code into iso */
        auto code = Builder::getASM(args);
        ISO::injectCode(code, args);
        if (!args.saveTemps) {Builder::cleanDirectory();}
    }    

    /* return without error */
    return 0;   
}
