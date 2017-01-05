#include "../HighLevel/HighLevel.h"
#include "../IsoHandling/ISO.h"
#include "../ArgumentParsing/Parser.h"
#include "../ArgumentParsing/Arguments.h"
#include "Description.h"

#include <iostream>
#include <algorithm>

int main(int argc, const char** argv)
{
    /* get command line arguments */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* parse meta options */
    CMDparser::parseMetaOptions(tokens);

    /* look for --save-temps option, erase it if given */ 
    Arguments args;
    auto pos = std::find(tokens.begin(), tokens.end(), "--save-temps");
    if (pos != tokens.end())
    {
        args.saveTemps = true;
        tokens.erase(pos);
    }    

    /* parse remaining two options */
    if (tokens.size() != 2)
    {
        throw std::invalid_argument("incorrect number of options");
        std::cout << Description::usage << std::endl;
        exit(0);
    }
    else 
    {
        /* parse file arguments */
        args.configFile = tokens.back();
        ConfigParser::parse(args);

        /* create iso handler */
        ISO iso (tokens.front());

        /* inject code into iso */
        auto code = Builder::getASM(args);
        iso.injectCode(code);
        if (!args.saveTemps) {Builder::cleanDirectory();}
    }    

    /* return without error */
    return 0;   
}
