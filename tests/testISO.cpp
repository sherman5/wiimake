#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE("create iso from reference")
{
    /* display header in first test case */
    displayHeader("Testing ISO.cpp");

    /* open up iso files */
    std::fstream referenceISO ("../tests/reference.iso");
    std::fstream testISO ("../tests/test.iso");

    /* loop through all addresses */
    uint32_t val;
    for (uint32_t a = 0; a < 0x457df2; a += 0x04)
    {
        referenceISO.read(reinterpret_cast<char *>(&val), sizeof(val));
        testISO.write(reinterpret_cast<char *>(&val), sizeof(val));
    }

    /* close iso files */
    referenceISO.close();
    testISO.close();
}

TEST_CASE("test all functions in ISO.cpp")
{
    /* populate args with info for iso */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));
    args.cmdOptions.insert(std::make_pair("--iso-file",
        "../tests/test.iso"));
    args.cmdOptions.insert(std::make_pair("--save",
        "../tests/save.data"));
    args.cmdOptions.insert(std::make_pair("--load",
        "../tests/save.data"));

    ConfigParser::parse(args);

    /** test dol offset calculation **/ 

    REQUIRE(ISO::DOLoffset(0x80003150, args.addressTable) == 0x150);
    REQUIRE(ISO::DOLoffset(0x80005570, args.addressTable) == 0x3b3e70);
    REQUIRE(ISO::DOLoffset(0x80005950, args.addressTable) == 0x2530);
    REQUIRE(ISO::DOLoffset(0x803b7290, args.addressTable) == 0x3b4290);
    REQUIRE(ISO::DOLoffset(0x804d79f0, args.addressTable) == 0x4313d0);

    /** test iso read **/

    REQUIRE(ISO::read(0x80003100, args) == 0x7c0802a6);
    REQUIRE(ISO::read(0x80003150, args) == 0x41820018);
    REQUIRE(ISO::read(0x80005570, args) == 0x20080000);
    REQUIRE(ISO::read(0x800056e0, args) == 0x80005530);
    REQUIRE(ISO::read(0x80006000, args) == 0xc0e10054);
    REQUIRE(ISO::read(0x803b8000, args) == 0xc091999a);
    REQUIRE(ISO::read(0x804d4000, args) == 0x6a6f626a);
    REQUIRE(ISO::read(0x804deb00, args) == 0x40000000);
    REQUIRE(ISO::read(0x804dec00, args) == 0xdeadbabe);

    REQUIRE(ISO::read(0x80191000, args) == ISO::read("80191000", args));

    /** test iso write */

    /* write value */
    ISO::write(0x80006000, 0x1234abcd, args);
    REQUIRE(ISO::read(0x80006000, args) == 0x1234abcd);

    /* replace with original code */
    ISO::write(0x80006000, 0xc0e10054, args);
    REQUIRE(ISO::read(0x80006000, args) == 0xc0e10054);

    /** test saving/loading **/

    /* write some unique values */
    ISO::write(0x80003100, 0x1234abcd, args);
    ISO::write(0x80006000, 0x1234abcd, args);
    ISO::write(0x803b8000, 0x1234abcd, args);
    ISO::write(0x804dec00, 0x1234abcd, args);

    /* save iso */
    ISO::saveState(args);

    /* revert values to original */
    ISO::write(0x80003100, 0x7c0802a6, args);
    ISO::write(0x80006000, 0xc0e10054, args);
    ISO::write(0x803b8000, 0xc091999a, args);
    ISO::write(0x804dec00, 0xdeadbabe, args);

    /* load iso */
    ISO::loadState(args);

    /* check that values changed */
    REQUIRE(ISO::read(0x80003100, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x80006000, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x803b8000, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x804dec00, args) == 0x1234abcd);

    /* revert values to original */
    ISO::write(0x80003100, 0x7c0802a6, args);
    ISO::write(0x80006000, 0xc0e10054, args);
    ISO::write(0x803b8000, 0xc091999a, args);
    ISO::write(0x804dec00, 0xdeadbabe, args);

    /* create code */
    ASMcode code;
    code.push_back(std::make_pair(0x80003100, 0x1234abcd));
    code.push_back(std::make_pair(0x80006000, 0x1234abcd));
    code.push_back(std::make_pair(0x803b8000, 0x1234abcd));
    code.push_back(std::make_pair(0x804dec00, 0x1234abcd));

    /* inject code */
    ISO::injectCode(code, args);
        
    /* check that values changed */
    REQUIRE(ISO::read(0x80003100, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x80006000, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x803b8000, args) == 0x1234abcd);
    REQUIRE(ISO::read(0x804dec00, args) == 0x1234abcd);

    /* revert values to original */
    ISO::write(0x80003100, 0x7c0802a6, args);
    ISO::write(0x80006000, 0xc0e10054, args);
    ISO::write(0x803b8000, 0xc091999a, args);
    ISO::write(0x804dec00, 0xdeadbabe, args);
}
