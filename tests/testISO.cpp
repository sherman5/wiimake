#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"

TEST_CASE("Testing functions in ISO.cpp")
{
    /* populate args with info for iso */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));
    args.cmdOptions.insert(std::make_pair("--iso-file",
        "../tests/test.iso"));

    ConfigParser::parse(args);

    SECTION("test DOL offset calculation")
    {
        REQUIRE(ISO::DOLoffset(0x80003150, args.addressTable) == 0x150);
        REQUIRE(ISO::DOLoffset(0x80005570, args.addressTable) == 0x3b3e70);
        REQUIRE(ISO::DOLoffset(0x80005950, args.addressTable) == 0x2530);
        REQUIRE(ISO::DOLoffset(0x803b7290, args.addressTable) == 0x3b4290);
        REQUIRE(ISO::DOLoffset(0x804d79f0, args.addressTable) == 0x4313d0);
    }

    SECTION("test iso read")
    {
        REQUIRE(ISO::read(0x80003100, args) == 0x7c0802a6);
        REQUIRE(ISO::read(0x80003150, args) == 0x41820018);
        REQUIRE(ISO::read(0x80005570, args) == 0x20080000);
        REQUIRE(ISO::read(0x800056e0, args) == 0x80005530);
        REQUIRE(ISO::read(0x80006000, args) == 0xc0e10054);
        REQUIRE(ISO::read(0x803b8000, args) == 0xc091999a);
        REQUIRE(ISO::read(0x804d4000, args) == 0x6a6f626a);
        REQUIRE(ISO::read(0x804deb00, args) == 0x40000000);
        REQUIRE(ISO::read(0x804dec00, args) == 0xdeadbabe);
    }

    SECTION("test iso read - string version")
    {
        REQUIRE(ISO::read(0x80191000, args) == ISO::read("80191000", args));
    }

    SECTION("test iso write")
    {
        /* write value */
        ISO::write(0x80006000, 0x1234abcd, args);
        REQUIRE(ISO::read(0x80006000, args) == 0x1234abcd);

        /* replace with original code */
        ISO::write(0x80006000, 0xc0e10054, args);
        REQUIRE(ISO::read(0x80006000, args) == 0xc0e10054);
    }

    SECTION("test saving/loading iso state")
    {
        /* set file name */
        args.cmdOptions.insert(std::make_pair("--save",
            "../tests/save.data"));

        args.cmdOptions.insert(std::make_pair("--load",
            "../tests/save.data"));


        /* write some unique values */
        ISO::write(0x80003100, 0x1234abcd, args);
        ISO::write(0x80006000, 0x1234abcd, args);
        ISO::write(0x803b8000, 0x1234abcd, args);
        ISO::write(0x804dec00, 0x1234abcd, args);

        /* save iso */
        //ISO::saveState(args);

        /* revert values to original */
        /*ISO::write(0x80003100, 0x7c0802a6, args);
        ISO::write(0x80006000, 0xc0e10054, args);
        ISO::write(0x803b8000, 0xc091999a, args);
        ISO::write(0x804dec00, 0xdeadbabe, args);

        /* load iso */
        //ISO::loadState(args);

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

    SECTION("Test injecting code into iso")
    {
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
}
