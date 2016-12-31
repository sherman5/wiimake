#include "catch.hpp"

#include "../tests/HeaderDisplay.h"
#include "../src/MainProgram/Parser.h"

TEST_CASE("parsing all command line options")
{
    /* display header in first test case */
    displayHeader("Testing CommandLineParser.cpp");

    /* get sample args */
    int argc = 28;
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

    /* get token list */
    TokenList tokens = CMDparser::getTokens(argc, argv);

    /* check that tokens are parsed correctly */
    REQUIRE(tokens.size() == 30);
    REQUIRE(tokens[0] == "--inject");
    REQUIRE(tokens[2] == "--save");
    REQUIRE(tokens[3] == "savefile.data");
    REQUIRE(tokens[8] == "--read");
    REQUIRE(tokens[9] == "80377008");

    /* check that options exist */
    REQUIRE(CMDparser::optionExists(tokens, "--save-temps"));
    REQUIRE(CMDparser::optionExists(tokens, "--config-file"));
    REQUIRE(CMDparser::optionExists(tokens, "--version"));
    REQUIRE(!CMDparser::optionExists(tokens, "programName"));

    /* check the getArg works */
    REQUIRE(CMDparser::getArg(tokens, "--save-temps") == "");
    REQUIRE(tokens.size() == 29);
    REQUIRE(CMDparser::getArg(tokens, "--ar") == "dir2/");
    REQUIRE(tokens.size() == 27);

    /* use to store arguments */
    Arguments args;

    /* parse options */
    REQUIRE_NOTHROW(CMDparser::parseOptions(tokens, args));

    REQUIRE(tokens.size() == 8);
    REQUIRE(args.cmdOptions.size() == 11);

    /* parse libs and includes */
    REQUIRE_NOTHROW(CMDparser::parseLibsAndIncludes(tokens, args));
    REQUIRE(tokens.size() == 0);
    REQUIRE(args.libs.size() == 2);
    REQUIRE(args.includePaths.size() == 2);

    /** test top level parse function **/
    args = Arguments();
    REQUIRE_NOTHROW(CMDparser::parse(argc, argv, args));
    REQUIRE(args.cmdOptions.size() == 13);  
    REQUIRE(args.libs.size() == 2);
    REQUIRE(args.includePaths.size() == 2);
}

TEST_CASE("handle unrecognized option")
{
    /* get cmd line args with error */
    int argc = 4;
    const char* argv[] =
    {
        "programName",
        "inject", "dir1/",
        "--save=savefile.data"
    }; 

    /* make sure invalid token isn't parsed */
    Arguments args;
    TokenList tokens = CMDparser::getTokens(argc, argv);
    CMDparser::parseOptions(tokens, args);
    CMDparser::parseLibsAndIncludes(tokens, args);
    REQUIRE(tokens.size() > 0);

    /* check that top level function throws error */
    REQUIRE_THROWS_AS(CMDparser::parse(argc, argv, args),
        std::invalid_argument);
}

TEST_CASE("handle no options")
{
    /* no cmd line args */
    int argc = 1;
    const char* argv[] =
    {
        "programName"
    }; 
        
    Arguments args;
    REQUIRE_NOTHROW(CMDparser::parse(argc, argv, args));
}
