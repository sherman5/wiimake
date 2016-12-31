#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/GCI.h"
#include "../src/MainProgram/Parser.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE("test that arguments are checked correctly")
{
    /* display header in the first test case */
    displayHeader("Testing GCI.cpp");

    /* struct to hold arguments */
    Arguments args;
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    /* populate with sample args, check for correct error handling */

    args.cmdOptions.insert(std::make_pair("--ar", "dir"));
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    args.cmdOptions.insert(std::make_pair("--output", ""));
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);
    
    args.cmdOptions["--output"] = "name";
    REQUIRE_NOTHROW(GCI::checkArgs(args));

    args.cmdOptions.insert(std::make_pair("--save", "file"));
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    args.cmdOptions.erase("--ar");
    args.cmdOptions.insert(std::make_pair("--config-file", "file"));
    args.cmdOptions.insert(std::make_pair("--iso-file", "file"));
    args.cmdOptions.insert(std::make_pair("--game-id", "id"));
    args.configOptions.insert(std::make_pair("code_start", 0xc));
    args.configOptions.insert(std::make_pair("code_end", 0xc));
    args.configOptions.insert(std::make_pair("DOL_start", 0xc)); 
    
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    args.addressTable.push_back(std::make_pair(0xa, 0xb));

    REQUIRE_NOTHROW(GCI::checkArgs(args));

    args.cmdOptions.insert(std::make_pair("--ar", ""));

    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);
    
    args.cmdOptions.erase("--ar");
    args.cmdOptions.erase("--save");
    args.cmdOptions.insert(std::make_pair("--inject", "dir"));
    
    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    args.configOptions.insert(std::make_pair("inject_address", 0xc)); 
    args.configOptions.insert(std::make_pair("original_instruction", 0xc)); 

    REQUIRE_THROWS_AS(GCI::checkArgs(args), std::invalid_argument);

    args.memRegions.push_back(MemRegion(0xa, 0xb));

    REQUIRE_NOTHROW(GCI::checkArgs(args));    
}

TEST_CASE("reading address from iso")
{
    /* populate arguments */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--inject", "../tests/source"));
    args.cmdOptions.insert(std::make_pair("--iso-file",
        "../tests/test.iso"));
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));
    ConfigParser::parse(args);

    /* read addresses */
    args.cmdOptions.insert(std::make_pair("--read", "80003100"));
    REQUIRE(GCI::readAddress(args) == 0x7c0802a6);

    args.cmdOptions["--read"] = "80006000";
    REQUIRE(GCI::readAddress(args) == 0xc0e10054);

    args.cmdOptions["--read"] = "803b8000";
    REQUIRE(GCI::readAddress(args) == 0xc091999a);

    args.cmdOptions["--read"] = "804dec00";
    REQUIRE(GCI::readAddress(args) == 0xdeadbabe);
}

TEST_CASE("saving/loading iso state & injecting code")
{
    /* populate arguments */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--inject", "../tests/source"));
    args.libs.push_back("../tests/lib1.a");
    args.includePaths.push_back("../tests/include");

    args.cmdOptions.insert(std::make_pair("--iso-file",
        "../tests/test.iso"));

    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));
    ConfigParser::parse(args);

    args.cmdOptions.insert(std::make_pair("--save", "savefile.data"));
    args.cmdOptions.insert(std::make_pair("--load", "savefile.data"));

    /* save, inject, load */

    REQUIRE_NOTHROW(GCI::saveISO(args));

    REQUIRE(ISO::read(0x803fa650, args) == 0x6974203e);
    REQUIRE(ISO::read(0x80300244, args) == 0x4e800020);
    REQUIRE(ISO::read(0x803fa714, args) == 0x3f800000);
    REQUIRE(ISO::read(0x803fa500, args) == 0x00000001);
    REQUIRE(ISO::read(0x803fa5a0, args) == 0x00000003);
    REQUIRE(ISO::read(0x80377998, args) == 0x7ee3bb78);

    REQUIRE_NOTHROW(GCI::createISO(args));

    REQUIRE(ISO::read(0x803fa650, args) == 0x9421ffe8);
    REQUIRE(ISO::read(0x80300244, args) == 0x4e800020);
    REQUIRE(ISO::read(0x803fa714, args) == 0x41000000);
    REQUIRE(ISO::read(0x803fa500, args) == 0x3920ffff);
    REQUIRE(ISO::read(0x803fa5a0, args) == 0xc8080208);
    REQUIRE(ISO::read(0x80377998, args) == 0x48082a51);

    REQUIRE_NOTHROW(GCI::loadISO(args));

    REQUIRE(ISO::read(0x803fa650, args) == 0x6974203e);
    REQUIRE(ISO::read(0x80300244, args) == 0x4e800020);
    REQUIRE(ISO::read(0x803fa714, args) == 0x3f800000);
    REQUIRE(ISO::read(0x803fa500, args) == 0x00000001);
    REQUIRE(ISO::read(0x803fa5a0, args) == 0x00000003);
    REQUIRE(ISO::read(0x80377998, args) == 0x7ee3bb78);
}
     

