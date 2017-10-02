#include "HighLevel.h"
#include "ISO.h"
#include "Parser.h"
#include "Arguments.h"
#include "Description.h"
#include "Global.h"

#include <iostream>
#include <algorithm>

static unsigned codeSize = 0;

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
    iso.injectCode(Builder::getZeroedMemory(args));

    iso.injectCode(Builder::getASM(args, codeSize));
    std::cout << "lines of code injected: " << codeSize / 4 << std::endl;
    std::cout << "percent of memory used: " << 
        100.f * (float) codeSize / Memory::totalSize() << std::endl;
    std::cout << "build checksum: " << std::hex << iso.checkSumDol() << std::endl;
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

    try
    {
        /* only two options should remain */
        INVALID_ARG(tokens.size() != 2, "incorrect number of options");

        /* run program */
        run(tokens);
    }
    catch (std::exception& e)
    {
        /* handle exception */
        std::cout << e.what() << std::endl;
        if (codeSize > 0 && Memory::totalSize() > 0)
        { 
            std::cout << "attempted number of lines of code: "
                << codeSize / 4 << std::endl;

            std::cout << "attempted percent of memory used: " << 
                100.f * (float) codeSize / Memory::totalSize() << std::endl;
        }
        std::cout << Description::usage << std::endl;
    }

    Builder::cleanDirectory(!saveTemps);

    return 0;   
}
