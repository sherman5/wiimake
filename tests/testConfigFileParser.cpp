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
    REQUIRE(tokens.size() == 32);   
    
    REQUIRE(tokens[5] == "803001dc-80301e40");
    REQUIRE(tokens[9] == "testSource2.c");
    REQUIRE(tokens[12] == "=");
    REQUIRE(tokens[19] == "-std=c99");
    REQUIRE(tokens[25] == "_main");
    REQUIRE(tokens[31] == "7ee3bb78");
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

TEST_CASE("store single string variable")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "ENTRY";
    TokenList values = {"test"};

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.entry == "test");
}

TEST_CASE("store numeric varaible")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "ADDRESS";
    TokenList values = {"1234abcd"};

    /* store variable */
    REQUIRE_NOTHROW(ConfigParser::storeVariable(args, name, values));

    /* check args */
    REQUIRE(args.injectAddress == 0x1234abcd);
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

TEST_CASE("store variable with too many values")
{
    /* create arguments struct */
    Arguments args;

    /* create data */
    std::string name = "ENTRY";
    TokenList values = {"first", "second"};

    /* store variable */
    REQUIRE_THROWS(ConfigParser::storeVariable(args, name, values));
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
    REQUIRE(args.memRegions[0].start == 0x803fa3e8);
    REQUIRE(args.memRegions[0].end == 0x803fc2ec);
    REQUIRE(args.memRegions[1].start == 0x803fc420);
    REQUIRE(args.memRegions[1].end == 0x803fdc1c);
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
    REQUIRE(args.memRegions[0].start == 0x803fa3e8);
    REQUIRE(args.memRegions[0].end == 0x803fc2ec);
    REQUIRE(args.memRegions[1].start == 0x803fc420);
    REQUIRE(args.memRegions[1].end == 0x803fdc1c);
    REQUIRE(args.memRegions[2].start == 0x801910e0);
    REQUIRE(args.memRegions[2].end == 0x80192930);
    REQUIRE(args.memRegions[3].start == 0x803001dc);
    REQUIRE(args.memRegions[3].end == 0x80301e40);

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
        
    REQUIRE(args.entry == "_main");
    
    REQUIRE(args.injectAddress == 0x80377998);
    REQUIRE(args.originalInstruction == 0x7ee3bb78);
}



