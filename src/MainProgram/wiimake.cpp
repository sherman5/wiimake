#include "../HighLevel/HighLevel.h"
#include "../ISO.h"
#include "../ArgumentParsing/Parser.h"
#include "../ArgumentParsing/Arguments.h"
#include "Description.h"
#include "../Global.h"

#include <iostream>
#include <algorithm>

void run(TokenList& tokens)
{
   /* look for --save-temps option, erase it if given */ 
    Arguments args;

    /* parse file arguments */
    args.configFile = tokens.back();
    ConfigParser::parse(args);

    /* create iso handler */
    ISO iso (tokens.front());

    /* inject code into iso */
    iso.injectCode(args.staticOverwrites);        
    auto code = Builder::getASM(args);
    iso.injectCode(code);
}

int main(int argc, const char** argv)
{
    /* get command line arguments, parse meta options */
    TokenList tokens = CMDparser::getTokens(argc, argv);
    CMDparser::parseMetaOptions(tokens);

    /* check if save-temps flag was given */
    bool saveTemps = false;
    auto pos = std::find(tokens.begin(), tokens.end(), "--save-temps");
    if (pos != tokens.end())
    {
        saveTemps = true;
        tokens.erase(pos);
    }    

    /* only two options should remain */
    INVALID_ARG(tokens.size() != 2, "incorrect number of options");

    try
    {
        /* run program */
        run(tokens);
    }
    catch (std::exception& e)
    {
        /* handle exception */
        std::cout << e.what() << std::endl;
        std::cout << Description::usage << std::endl;
    }

    if (!saveTemps) {Builder::cleanDirectory();}
    return 0;   
}
