#include "catch.hpp"

#include "HeaderDisplay.h"
#include "../src/ISO.h"
#include "../src/LowLevel/LowLevel.h"

static const std::string path = "../tests/files/ISO/";

TEST_CASE("test all functions in ISO.cpp")
{
    /* display header in first test case */
    displayHeader("Testing ISO.cpp");

    /* creat iso for testing */
    System::runCMD(System::rm + " " + path + "test.iso");
    System::runCMD(System::cp + " ../tests/files/reference.iso "
        + path + "test.iso");

    /* create iso object */
    ISO iso(path + "test.iso");

    /* check DOL offset calculations */
    REQUIRE(iso.dolOffset(0x80003150) == 0x1e950);
    REQUIRE(iso.dolOffset(0x80005570) == 0x3d2670);
    REQUIRE(iso.dolOffset(0x80005950) == 0x20d30);
    REQUIRE(iso.dolOffset(0x803b7290) == 0x3d2a90);
    REQUIRE(iso.dolOffset(0x804d79f0) == 0x44fbd0);

    /** test iso read **/
    REQUIRE(iso.read(0x80003100) == 0x7c0802a6);
    REQUIRE(iso.read(0x80003150) == 0x41820018);
    REQUIRE(iso.read(0x80005570) == 0x20080000);
    REQUIRE(iso.read(0x800056e0) == 0x80005530);
    REQUIRE(iso.read(0x80006000) == 0xc0e10054);
    REQUIRE(iso.read(0x803b8000) == 0xc091999a);
    REQUIRE(iso.read(0x804d4000) == 0x6a6f626a);
    REQUIRE(iso.read(0x804deb00) == 0x40000000);
    REQUIRE(iso.read(0x804dec00) == 0xdeadbabe);
    REQUIRE(iso.read(0x80191000) == iso.read("80191000"));
    REQUIRE_NOTHROW(iso.read(0x1e800));

    /* write value */
    iso.write(0x80006000, 0x1234abcd);
    REQUIRE(iso.read(0x80006000) == 0x1234abcd);

    /* replace with original code */
    iso.write(0x80006000, 0xc0e10054);
    REQUIRE(iso.read(0x80006000) == 0xc0e10054);

    /* write some unique values */
    iso.write(0x80003100, 0x1234abcd);
    iso.write(0x80006000, 0x1234abcd);
    iso.write(0x803b8000, 0x1234abcd);
    iso.write(0x804dec00, 0x1234abcd);

    /* save iso */
    iso.saveState("../tests/files/ISO/save.data");

    /* revert values to original */
    iso.write(0x80003100, 0x7c0802a6);
    iso.write(0x80006000, 0xc0e10054);
    iso.write(0x803b8000, 0xc091999a);
    iso.write(0x804dec00, 0xdeadbabe);

    /* load iso */
    iso.loadState("../tests/files/ISO/save.data");

    /* check that values changed */
    REQUIRE(iso.read(0x80003100) == 0x1234abcd);
    REQUIRE(iso.read(0x80006000) == 0x1234abcd);
    REQUIRE(iso.read(0x803b8000) == 0x1234abcd);
    REQUIRE(iso.read(0x804dec00) == 0x1234abcd);

    /* revert values to original */
    iso.write(0x80003100, 0x7c0802a6);
    iso.write(0x80006000, 0xc0e10054);
    iso.write(0x803b8000, 0xc091999a);
    iso.write(0x804dec00, 0xdeadbabe);

    /* create code */
    ASMcode code;
    code.push_back(std::make_pair(0x80003100, 0x1234abcd));
    code.push_back(std::make_pair(0x80006000, 0x1234abcd));
    code.push_back(std::make_pair(0x803b8000, 0x1234abcd));
    code.push_back(std::make_pair(0x804dec00, 0x1234abcd));

    /* inject code */
    iso.injectCode(code);
        
    /* check that values changed */
    REQUIRE(iso.read(0x80003100) == 0x1234abcd);
    REQUIRE(iso.read(0x80006000) == 0x1234abcd);
    REQUIRE(iso.read(0x803b8000) == 0x1234abcd);
    REQUIRE(iso.read(0x804dec00) == 0x1234abcd);

    /* revert values to original */
    iso.write(0x80003100, 0x7c0802a6);
    iso.write(0x80006000, 0xc0e10054);
    iso.write(0x803b8000, 0xc091999a);
    iso.write(0x804dec00, 0xdeadbabe);
}

    

