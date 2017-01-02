#include "catch.hpp"

#include "../src/MainProgram/Parser.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE("correct game is found")
{
    /* display header in first test case */
    displayHeader("Testing ConfigFileParser.cpp");

    /* arguments used for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));

    /* go one line past game_id */
    std::ifstream file ("../tests/config.ini", std::ios::in);
    ConfigParser::findGame(args, file);
    std::string line;
    file >> line;

    /* this string was put in a comment for testing */
    REQUIRE(line == "CORRECT");
}

TEST_CASE("parse table")
{
    /* arguments used for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));

    /* find starting point in config file */
    std::ifstream file("../tests/config.ini", std::ios::in);
    ConfigParser::findGame(args, file);
    std::string line;

    /* read until memory_regions variable is found */
    while (line != "memory_regions=") { file >> line;}

    /* store regions */
    ConfigParser::storeMemoryRegions(file, args);

    /* check values */
    REQUIRE(args.memRegions[0] == MemRegion(0x803fa3e8, 0x803fc2ec));
    REQUIRE(args.memRegions[1] == MemRegion(0x803fc420, 0x803fdc1c));
    REQUIRE(args.memRegions[2] == MemRegion(0x801910e0, 0x80192930));
    REQUIRE(args.memRegions[3] == MemRegion(0x803001dc, 0x80301e40));
}

TEST_CASE("parse single option")
{
    /* arguments used for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));

    /* find starting point in config file */
    std::ifstream file ("../tests/config.ini", std::ios::in);
    ConfigParser::findGame(args, file);
    std::string line;

    /* read until code_end variable is found */
    while (line.find("end=") == std::string::npos) { file >> line;}

    /* store variable */
    ConfigParser::storeVariable(file, line, args);

    /* check values */      
    REQUIRE(args.configOptions["code_end"] == 0x804dec00);
}

TEST_CASE("parse full config file")
{
    /* arguments used for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));

    /* parse all args */
    ConfigParser::parse(args);

    /* check values */
    REQUIRE(args.configOptions.size() == 5);
    REQUIRE(args.configOptions["code_start"] == 0x80003100);
    REQUIRE(args.configOptions["code_end"] == 0x804dec00);
    REQUIRE(args.configOptions["inject_address"] == 0x80377998);
    REQUIRE(args.configOptions["original_instruction"] == 0x7ee3bb78);
    REQUIRE(args.configOptions["DOL_start"] == 0x1e800);

    REQUIRE(args.memRegions[0] == MemRegion(0x803fa3e8, 0x803fc2ec));
    REQUIRE(args.memRegions[1] == MemRegion(0x803fc420, 0x803fdc1c));
    REQUIRE(args.memRegions[2] == MemRegion(0x801910e0, 0x80192930));
    REQUIRE(args.memRegions[3] == MemRegion(0x803001dc, 0x80301e40));

    REQUIRE(args.addressTable.size() == 6);
    REQUIRE(args.addressTable[0].first == 0x000100);
    REQUIRE(args.addressTable[0].second == 0x80003100);
    REQUIRE(args.addressTable[3].first == 0x3b4240);
    REQUIRE(args.addressTable[3].second == 0x803b7240);
    REQUIRE(args.addressTable[5].first == 0x4313c0);
    REQUIRE(args.addressTable[5].second == 0x804d79e0);
}

TEST_CASE("error when value is missing in variable")
{
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));
    std::ifstream file ("../tests/config.ini", std::ios::in);
    ConfigParser::findGame(args, file);
    std::string line = "variable=";

    REQUIRE_THROWS_AS(ConfigParser::storeVariable(file, line, args),
        std::invalid_argument);
}

TEST_CASE("no config file provided")
{
    Arguments args;
    REQUIRE_NOTHROW(ConfigParser::parse(args));

    args.cmdOptions.insert(std::make_pair("--config-file", ""));
    REQUIRE_NOTHROW(ConfigParser::parse(args));
}

TEST_CASE("test getting game titles")
{
    /* get titles */
    auto list = ConfigParser::getGameTitles("../tests/config.ini");

    REQUIRE(list.size() == 3);
    REQUIRE(list[0] == "Dummy 1");
    REQUIRE(list[1] == "Test Config");
    REQUIRE(list[2] == "Dummy 2");
}




