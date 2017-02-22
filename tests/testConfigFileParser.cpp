#include "catch.hpp"

#include "../src/ArgumentParsing/Parser.h"
#include "../src/ArgumentParsing/Arguments.h"
#include "HeaderDisplay.h"

TEST_CASE("convert file to tokens")
{
    /* display header in first test case */
    displayHeader("Testing ConfigFileParser.cpp");

    /* get tokens */
    TokenList tokens = ConfigParser::getTokens(
        "../tests/files/ConfigFileParser/config.ini");

    /* check values */
    REQUIRE(tokens.size() == 34);   
    
    REQUIRE(tokens[5] == "803001dc-80301e40");
    REQUIRE(tokens[9] == "testSource2.c");
    REQUIRE(tokens[12] == "=");
    REQUIRE(tokens[19] == "-std=c99");
    REQUIRE(tokens[25] == "_main");
    REQUIRE(tokens[27] == "7ee3bb78");
}

TEST_CASE("store single variable")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "LINKER_FLAGS";
    TokenList values = {"-flag1", "-flag-2", "-flag3"};

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.linkFlags.size() == 3);
    REQUIRE(args.linkFlags[0] == "-flag1");
    REQUIRE(args.linkFlags[1] == "-flag-2");
    REQUIRE(args.linkFlags[2] == "-flag3");
}

TEST_CASE("store fixed symbols list")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "FIXED_SYMBOLS";
    TokenList values = {"_sym1", "80105000", "abcdabcd", "_sym2",
        "80030040", "deadbabe"};

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.fixedSymbols.size() == 2);
    REQUIRE(args.fixedSymbols[0].name == "_sym1");
    REQUIRE(args.fixedSymbols[1].address == 0x80030040);
    REQUIRE(args.fixedSymbols[1].instruction == 0xdeadbabe);
}

TEST_CASE("store variable with empty values")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "SOURCES";
    TokenList values;

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.sources.empty());
}

TEST_CASE("store invalid variable")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "SOURCESSS";
    TokenList values = {"source1"};

    /* store variable */
    REQUIRE_THROWS(ConfigParser::storeVariable(args, name, values));
}

TEST_CASE("storing memory regions")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "REGIONS";
    TokenList values = {"803fa3e8-803fc2ec", "803fc420-803fdc1c"};

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.memRegions.size() == 2);
    REQUIRE(args.memRegions[0].start == 0x803fc424);
    REQUIRE(args.memRegions[0].end == 0x803fdc18);
    REQUIRE(args.memRegions[1].start == 0x803fa3ec);
    REQUIRE(args.memRegions[1].end == 0x803fc2e8);
}

TEST_CASE("store entire config file")
{
    /* create arguments */
    Arguments args;
    args.configFile = "../tests/files/ConfigFileParser/config.ini";

    /* parse file */
    ConfigParser::parse(args);

    /* check args */
    REQUIRE(args.memRegions.size() == 4);
    REQUIRE(args.memRegions[0].start == 0x803fc424);
    REQUIRE(args.memRegions[0].end == 0x803fdc18);
    REQUIRE(args.memRegions[1].start == 0x801910e4);
    REQUIRE(args.memRegions[1].end == 0x8019292c);
    REQUIRE(args.memRegions[2].start == 0x803001e0);
    REQUIRE(args.memRegions[2].end == 0x80301e3c);
    REQUIRE(args.memRegions[3].start == 0x803fa3ec);
    REQUIRE(args.memRegions[3].end == 0x803fc2e8);

    REQUIRE(args.sources.size() == 3);
    REQUIRE(args.sources[0] == "testSource1.c");
    REQUIRE(args.sources[1] == "testSource2.c");
    REQUIRE(args.sources[2] == "../path/testSource3.c");

    REQUIRE(args.libs.size() == 2);
    REQUIRE(args.libs[0] == "testlib1.a");
    REQUIRE(args.libs[1] == "../path/testlib2.a");

    REQUIRE(args.includePaths.empty());
       
    REQUIRE(args.compileFlags.size() == 2);
    REQUIRE(args.compileFlags[0] == "-std=c99");
    
    REQUIRE(args.linkFlags.empty());
        
    REQUIRE(args.fixedSymbols.size() == 1);
    REQUIRE(args.fixedSymbols[0].name == "_main");
    REQUIRE(args.fixedSymbols[0].address == 0x80377998);
    REQUIRE(args.fixedSymbols[0].instruction == 0x7ee3bb78);

    REQUIRE(args.staticOverwrites[0].first == 0x8045bf28);
    REQUIRE(args.staticOverwrites[1].first == 0x8045bf2c);
    REQUIRE(args.staticOverwrites[1].second == 0xffffffff);
}


