#include "catch.hpp"

#include "../src/HighLevel/HighLevel.h"
#include "../src/ArgumentParsing/Parser.h"
#include "HeaderDisplay.h"

#include <algorithm>
#include <string>

static const std::string path = "../tests/files/Builder/";

/* ignore back/forward slashes in windows */
#ifdef WIIMAKE_WINDOWS

    bool STR_EQ(std::string a, std::string b)
    {
        std::replace(a.begin(), a.end(), '/', 'x');
        std::replace(a.begin(), a.end(), '\\', 'x');
    
        std::replace(b.begin(), b.end(), '/', 'x');
        std::replace(b.begin(), b.end(), '\\', 'x');

        return a == b;
    }

#else
    
    bool STR_EQ(std::string a, std::string b) {return a == b;}

#endif

TEST_CASE("add overwritten asm lines")
{
    /* display header in first test case */
    displayHeader("Testing Builder.cpp");

    /* needed args */
    Arguments args;
    args.fixedSymbols.push_back(FixedSymbol("sym1", 0x125, 0xe1));
    args.fixedSymbols.push_back(FixedSymbol("sym2", 0x125, 0xe2));
    args.memRegions.push_back(MemRegion(0x120, 0x200));

    /* create sample code */
    ASMcode testCode;
    testCode.push_back(std::make_pair(0x50, 0xabc));        
    testCode.push_back(std::make_pair(0x75, 0xabc));
    testCode.push_back(std::make_pair(0x100, 0xabc));
    testCode.push_back(std::make_pair(0x124, 0x60000000));
    testCode.push_back(std::make_pair(0x130, 0x60000000));
    testCode.push_back(std::make_pair(0x150, 0xabc));
    testCode.push_back(std::make_pair(0x200, 0xabc));

    /* overwrite nop */
    //REQUIRE_NOTHROW(Builder::addOverwrittenASM(testCode, args));
    REQUIRE(testCode[3].first == 0x124);
    REQUIRE(testCode[3].second == 0xe1);
    REQUIRE(testCode[4].first == 0x130);
    REQUIRE(testCode[4].second == 0xe2);

    /* should return error - instruction not nop */
    //REQUIRE_THROWS_AS(Builder::addOverwrittenASM(testCode, args),
    //    std::runtime_error);
}

TEST_CASE("get asm code to inject")
{
    /* needed args */
    Arguments args;
    args.sources.push_back(path + "source/source1.c");
    args.sources.push_back(path + "source/source2.c");
    args.libs.push_back(path + "lib1.a");
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

    REQUIRE(code.size() == 247);

    REQUIRE(code[0].first == 0x803fa644);
    REQUIRE(code[0].second == 0x9421ffe8);

    REQUIRE(code[67].first == 0x8030025c);
    REQUIRE(code[67].second == 0x4e800020);

    REQUIRE(code[68].first == 0x803fa720);
    REQUIRE(code[68].second == 0x41000000);

    REQUIRE(code[139].first == 0x803fa4f4);
    REQUIRE(code[139].second == 0x3920ffff);

    REQUIRE(code[179].first == 0x803fa594);
    REQUIRE(code[179].second == 0xc8080220);

    REQUIRE(code[239].first == 0x80377998);
    REQUIRE(code[239].second == 0x4bf88844);

    REQUIRE(code[246].first == 0x803001f0);
    REQUIRE(code[246].second == 0x4bd01614);

    //Builder::cleanDirectory();
}
