#include "../ArgumentParsing/Parser.h"
#include "../IsoHandling/ISO.h"
#include "Description.h"

#include <iostream>

void run(TokenList& tokens)
{
    /* check for correct number of arguments */
    if (tokens.size() != 3)
    {
        throw std::invalid_argument("incorrect number of arguments");
    }
    
    /* create iso handler */
    ISO iso (tokens[0]);
    
    /* run the program */
    if (tokens[1] == "--save")
    {
        iso.saveState(tokens.back());
    }
    else if (tokens[1] == "--load")
    {
        iso.loadState(tokens.back());
    }
    else if (tokens[1] == "--read")
    {
        /* make sure valid address is given */
        try
        {
            std::cout << std::hex << iso.read(tokens.back()) << std::endl;
        }
        catch (std::invalid_argument& e)
        {
            if (std::string(e.what()) == "stoul")
            {
                throw std::invalid_argument("invalid address: "
                    + tokens.back());
            }
            else
            {
                throw e;
            }
        }
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
