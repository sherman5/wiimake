#include "catch.hpp"

#include <iostream>
#include "../src/MainProgram/Parser.h"

TEST_CASE("Testing functions in CommandLineParser.cpp")
{
    /* get sample args */
    const char* argv[] =
    {
        "programName",
        "--inject", "dir1/",
        "--save=savefile.data",
        "--load", "loadfile.data",
        "--ar", "dir2/",
        "--read=80377008",
        "--config-file=config.ini",
        "--iso-file", "game.iso", 
        "--output", "libname.a",
        "--save-temps",
        "--game-id", "2",
        "--help", 
        "--usage", 
        "--version",
        "--lib", "lib1.a",
        "--lib", "lib2.a",
        "--include", "incl1/",
        "--include", "incl2/"
    }; 

    int argc = 28;

    /* get token list */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    SECTION("Test breaking up command line arguments into tokens")
    {
        REQUIRE(tokens.size() == 30);
        REQUIRE(tokens[0] == "--inject");
        REQUIRE(tokens[2] == "--save");
        REQUIRE(tokens[3] == "savefile.data");
        REQUIRE(tokens[8] == "--read");
        REQUIRE(tokens[9] == "80377008");
    }

    SECTION("Test option recognition")
    {
        REQUIRE(CMDparser::optionExists(tokens, "--save-temps"));
        REQUIRE(CMDparser::optionExists(tokens, "--config-file"));
        REQUIRE(CMDparser::optionExists(tokens, "--version"));
        REQUIRE(!CMDparser::optionExists(tokens, "programName"));
    }

    SECTION("Test retrieving an argument")
    {
        REQUIRE(CMDparser::getArg(tokens, "--save-temps") == "");
        REQUIRE(tokens.size() == 29);
        REQUIRE(CMDparser::getArg(tokens, "--ar") == "dir2/");
        REQUIRE(tokens.size() == 27);
    }

    /* use to store arguments */
    Arguments args;

    SECTION("Test option parsing")
    {
        /* parse options */
        REQUIRE_NOTHROW(CMDparser::parseOptions(tokens, args));

        REQUIRE(tokens.size() == 8);
        REQUIRE(args.cmdOptions.size() == 13);
        
    }

    SECTION("Test lib/include parsing")
    {
        /* parse libs and includes */
        REQUIRE_NOTHROW(CMDparser::parseLibsAndIncludes(tokens, args));
        
        REQUIRE(tokens.size() == 22);
        REQUIRE(args.libs.size() == 2);
        REQUIRE(args.includePaths.size() == 2);
    }

    SECTION("Test top level parse function")
    {
        /* parse all options */
        REQUIRE_NOTHROW(CMDparser::parse(argc, argv, args));
        REQUIRE(args.cmdOptions.size() == 13);  
        REQUIRE(args.libs.size() == 2);
        REQUIRE(args.includePaths.size() == 2);
    }

    SECTION("Test error recognition")
    {
        /* change option to error */
        argv[26] = "-include";

        /* make sure invalid token isn't parsed */
        tokens = CMDparser::getTokens(argc, argv);
        CMDparser::parseOptions(tokens, args);
        CMDparser::parseLibsAndIncludes(tokens, args);

        REQUIRE(tokens.size() > 0);

        /* check that top level function throws error */
        REQUIRE_THROWS_AS(CMDparser::parse(argc, argv, args),
            std::invalid_argument);
    }
    
}
