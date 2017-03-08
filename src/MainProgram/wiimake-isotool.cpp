#include "../ArgumentParsing/Parser.h"
#include "../ISO.h"
#include "../Global.h"
#include "Description.h"

#include <iostream>

void run(TokenList& tokens)
{
    //TODO: handle invalid file
    /* create iso handler */
    ISO iso (tokens[0]);
    
    /* run the program */
    if (tokens[1] == "--read")
    {
        INVALID_ARG(tokens.size() != 3, "incorrect number of arguments");
        /* make sure valid address is given */
        try
        {
            std::cout << std::hex << iso.read(tokens.back()) << std::endl;
        }
        catch (std::invalid_argument& e)
        {
            INVALID_ARG(std::string(e.what()) == "stoul",
                "invalid address: " + tokens.back());

            throw e;
        }
    }
    else if (tokens[1] == "--checksum")
    {
        INVALID_ARG(tokens.size() != 2, "incorrect number of arguments");
        std::cout << std::hex << iso.checkSum() << std::endl;
    }
    else if (tokens[1] == "--save")
    {
        INVALID_ARG(tokens.size() != 3, "incorrect number of arguments");
        iso.saveState(tokens.back());
    }
    else if (tokens[1] == "--load") 
    {
        INVALID_ARG(tokens.size() != 3, "incorrect number of arguments");
        iso.loadState(tokens.back());
    }
    else
    {
        throw std::invalid_argument("invalid option: " + tokens[1]);
    }
}

int main(int argc, const char** argv)
{
    try
    {
        /* get command line arguments */
        TokenList tokens = CMDparser::getTokens(argc, argv);

        /* parse meta options */
        CMDparser::parseMetaOptions(tokens);

        /* run program */
        run(tokens);
    }
    catch (std::exception& e)
    {
        /* handle error */
        std::cout << e.what() << std::endl;
        std::cout << Description::usage << std::endl;
        return 1;
    }

    /* return without error */
    return 0;
}
