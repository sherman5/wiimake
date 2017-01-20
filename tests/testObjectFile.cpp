#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

static const std::string path = "../tests/files/ObjectFile/";

TEST_CASE("break object file into tokens")
{
    /* display header in first test case */
    displayHeader("Testing ObjectFile.cpp");

    auto tokens = ObjectFile::getTokens(path + "objectFile.out");

    REQUIRE(tokens.size() == 598);

    REQUIRE(tokens[1] == "file");
    REQUIRE(tokens[10] == "803fa4f0:");
    REQUIRE(tokens[30] == ".long");
    REQUIRE(tokens[80] == ".long");
    REQUIRE(tokens[150] == "r9,-32704");
    REQUIRE(tokens[250] == "803fa470");
    REQUIRE(tokens[400] == "07");
    REQUIRE(tokens[597] == "bounds.");
}

TEST_CASE("recognize single line of code")
{

    REQUIRE(!ObjectFile::lineOfCode(""));    
    REQUIRE(!ObjectFile::lineOfCode("a.out:"));
    REQUIRE(!ObjectFile::lineOfCode("gci_1:"));
    REQUIRE(!ObjectFile::lineOfCode("<gci_1>:"));
    REQUIRE(ObjectFile::lineOfCode("0:"));
    REQUIRE(ObjectFile::lineOfCode("c:"));
    REQUIRE(!ObjectFile::lineOfCode("<TimesTwo>:"));
    REQUIRE(!ObjectFile::lineOfCode(".comment:"));
}

TEST_CASE("parse single line of code")
{
    auto tokens = ObjectFile::getTokens(path + "objectFile.out");

    auto it = tokens.begin() + 10;
    auto code = ObjectFile::getCode(it);

    REQUIRE(code.first == 0x803fa4f0);
    REQUIRE(code.second == 0x41000000);
}

TEST_CASE("extract asm from object file")
{
   /* get asm code */
    auto code = ObjectFile::extractASM(path + "objectFile.out");

    /* check number of lines */
    REQUIRE(code.size() == 77);

    /* check some lines of code */
    REQUIRE(code[0].first == 0x803fa4f0);
    REQUIRE(code[0].second == 0x41000000);
    REQUIRE(code[4].first == 0x803fa4dc);
    REQUIRE(code[9].second == 0x9421ffe8);
    REQUIRE(code[67].first == 0x803fa3f0);
    REQUIRE(code[67].second == 0x4bf7d5ac);
}

TEST_CASE("get sections from object file")
{
    /* get sections */
    auto sect = ObjectFile::getSections(path + "objectFile.out");

    /* check number of sections */
    REQUIRE(sect.size() == 8);

    /* check some section names */
    REQUIRE(sect[0] == "gci_1");
    REQUIRE(sect[6] == "gci_7");
    REQUIRE(sect[7] == ".comment");
}

