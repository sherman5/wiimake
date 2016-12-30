#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

TEST_CASE ("testing functions in ObjectFile.cpp")
{
    SECTION("Recognize line of asm code")
    {
        REQUIRE(!ObjectFile::lineOfCode("a.out:"));
        REQUIRE(!ObjectFile::lineOfCode("gci_1:"));
        REQUIRE(!ObjectFile::lineOfCode("<gci_1>:"));
        REQUIRE(ObjectFile::lineOfCode("0:"));
        REQUIRE(ObjectFile::lineOfCode("c:"));
        REQUIRE(!ObjectFile::lineOfCode("<TimesTwo>:"));
        REQUIRE(!ObjectFile::lineOfCode(".comment:"));
    }

    SECTION("Test single line parsing (from text file)")
    {
        /* open up test file */
        std::ifstream file ("../tests/objectFile.txt");

        /* string to store line */
        std::string line;
        
        /* find first line of code */
        while (!ObjectFile::lineOfCode(line))
        {
            file >> line;
        }
        
        /* first line of code - (803fa4f0, 41000000) */
        auto code = ObjectFile::getLine(line, file);
        REQUIRE(code.first == 0x803fa4f0);
        REQUIRE(code.second == 0x41000000);
    }

    SECTION("Test extracting asm (from object file)")
    {
        /* get asm code */
        auto code = ObjectFile::extractASM("../tests/objectFile.out");

        /* check number of lines */
        REQUIRE(code.size() == 68);

        /* check some lines of code */
        REQUIRE(code[0].first == 0x803fa4f0);
        REQUIRE(code[0].second == 0x41000000);
        REQUIRE(code[4].first == 0x803fa4dc);
        REQUIRE(code[9].second == 0x9421ffe8);
        REQUIRE(code[67].first == 0x803fa3f0);
        REQUIRE(code[67].second == 0x4bf7d5ac);
    }

    SECTION("Test renaming sections")
    {
        // untested for now - just runs an explicit system command
    }

    SECTION("Test finding named sections in object file")
    {
        /* get named sections */
        auto sect = ObjectFile::getNamedSections("../tests/objectFile.out");

        /* check number of sections */
        REQUIRE(sect.size() == 7);

        /* check some section names */
        REQUIRE(sect[0] == "gci_1");
        REQUIRE(sect[6] == "gci_7");

    }
}

