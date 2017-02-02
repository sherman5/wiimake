#include "catch.hpp"

#include "../src/HighLevel/HighLevel.h"
#include "../src/ArgumentParsing/Parser.h"
#include "HeaderDisplay.h"

#include <algorithm>
#include <string>

static const std::string path = "../tests/files/Builder/";

TEST_CASE("add overwritten asm lines")
{
    /* display header in first test case */
    displayHeader("Testing Builder.cpp");

    /* needed args */
    Arguments args;
    args.fixedSymbols.push_back(FixedSymbol("sym1", 0, 0xe1));
    args.fixedSymbols.push_back(FixedSymbol("sym2", 0, 0xe2));
    
    SectionList sections;
    sections.push_back(Section("stack_setup_0.o", 0x100));
    sections.push_back(Section("stack_setup_1.o", 0x200));

    /* create sample code */
    ASMcode testCode;
    testCode.push_back(std::make_pair(0x50, 0xabc));        
    testCode.push_back(std::make_pair(0x75, 0xabc));
    testCode.push_back(std::make_pair(0x100, 0xabc));
    testCode.push_back(std::make_pair(0x14c, 0x60000000));
    testCode.push_back(std::make_pair(0x200, 0xabc));
    testCode.push_back(std::make_pair(0x24c, 0x60000000));
    testCode.push_back(std::make_pair(0x300, 0xabc));


    /* overwrite nop */
    REQUIRE_NOTHROW(Builder::addOverwrittenASM(testCode, args, sections));
    REQUIRE(testCode[3].first == 0x14c);
    REQUIRE(testCode[3].second == 0xe1);
    REQUIRE(testCode[5].first == 0x24c);
    REQUIRE(testCode[5].second == 0xe2);

    /* should return error - instruction not nop */
    REQUIRE_THROWS_AS(Builder::addOverwrittenASM(testCode, args, sections),
        std::runtime_error);
}

TEST_CASE("get asm code to inject")
{
    /* needed args */
    Arguments args;
    args.sources.push_back(path + "source/source1.c");
    args.sources.push_back(path + "source/source2.c");
    args.libs.push_back(path + "libmath.a");
    args.includePaths.push_back(path + "include");
    args.fixedSymbols.push_back(FixedSymbol("_main", 0x80377998,
        0x7ee3bb78));
    args.fixedSymbols.push_back(FixedSymbol("TimesTwo", 0x80001800,
        0xabcdabd));
    args.memRegions.push_back(MemRegion(0x803fa3e8, 0x803fc2ec));
    args.memRegions.push_back(MemRegion(0x803fc420, 0x803fdc1c));
    args.memRegions.push_back(MemRegion(0x801910e0, 0x80192930));
    args.memRegions.push_back(MemRegion(0x803001dc, 0x80301e40));

    /* compile source files, return object files */
    auto code = Builder::getASM(args);

    REQUIRE(code.size() == 120);

    REQUIRE(code[0].first == 0x80377998);
    REQUIRE(code[0].second == 0x48082b10);

    REQUIRE(code[21].first == 0x803fa4f8);
    REQUIRE(code[21].second == 0x4bf7d4a4);

    REQUIRE(code[22].first == 0x80001800);
    REQUIRE(code[22].second == 0x483f8d00);

    REQUIRE(code[60].first == 0x803fa428);
    REQUIRE(code[60].second == 0x4fddf382);

    REQUIRE(code[85].first == 0x803fa48c);
    REQUIRE(code[85].second == 0x800b0004);

    REQUIRE(code[100].first == 0x803fa558);
    REQUIRE(code[100].second == 0x9421ffe8);

    REQUIRE(code[119].first == 0x803fa5d4);
    REQUIRE(code[119].second == 0x00070401);

    Builder::cleanDirectory();
}
