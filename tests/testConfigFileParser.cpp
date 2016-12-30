#include "catch.hpp"

#include "../src/MainProgram/Parser.h"

TEST_CASE("Testing functions in ConfigFileParser.cpp")
{
    /* arguments used for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));

    SECTION("Test that correct game is found")
    {
        /* read one more line past */
        std::ifstream file = ConfigParser::findGame(args);
        std::string line;
        file >> line;

        /* this string was put in a comment for testing */
        REQUIRE(line == "CORRECT");
    }

    /* from here, use this file stream as starting point */
    std::ifstream file = ConfigParser::findGame(args);
    std::string line;

    SECTION("Test that tables are parsed correctly")
    {
        /* read until table is found */
        while (line != "memory_regions=") { file >> line;}

        /* get table */
        Table table = ConfigParser::getTable(file);
    
        /* check values */
        REQUIRE(table[0].first == 0x803fa3e8);
        REQUIRE(table[0].second == 0x803fc2ec);
        REQUIRE(table[1].first == 0x803fc420);
        REQUIRE(table[1].second == 0x803fdc1c);
        REQUIRE(table[2].first == 0x801910e0);
        REQUIRE(table[2].second == 0x80192930);
        REQUIRE(table[3].first == 0x803001dc);
        REQUIRE(table[3].second == 0x80301e40);
    }

    SECTION("Test that memory regions are parsed correctly")
    {
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

    SECTION("Test that generic variables are stored correctly")
    {
        /* read until code_end variable is found */
        while (line.find("end=") == std::string::npos) { file >> line;}

        /* store variable */
        ConfigParser::storeVariable(file, line, args);

        /* check values */      
        REQUIRE(args.configOptions["code_end"] == 0x804dec00);

        /* read until memory_regions variable is found */
        while (line != "memory_regions=") { file >> line;}

        /* store regions */
        ConfigParser::storeVariable(file, line, args);

        /* check values */
        REQUIRE(args.memRegions[0] == MemRegion(0x803fa3e8, 0x803fc2ec));
        REQUIRE(args.memRegions[1] == MemRegion(0x803fc420, 0x803fdc1c));
        REQUIRE(args.memRegions[2] == MemRegion(0x801910e0, 0x80192930));
        REQUIRE(args.memRegions[3] == MemRegion(0x803001dc, 0x80301e40));
    }

    SECTION("Test top level parse function")
    {
        /* parse config file */
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
}




