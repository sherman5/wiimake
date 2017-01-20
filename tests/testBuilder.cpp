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
    
    bool STR_EQ(std::string a, std::string b)
    {
        return a == b;
    }

#endif

TEST_CASE("add original instruction")
{
    /* needed args */
    Arguments args;
    args.injectAddress = 0x125;
    args.originalInstruction = 0xe;
    args.memRegions.push_back(MemRegion(0x120, 0x200));

    /* create sample code */
    ASMcode testCode;
    testCode.push_back(std::make_pair(0x50, 0xabc));        
    testCode.push_back(std::make_pair(0x75, 0xabc));
    testCode.push_back(std::make_pair(0x100, 0xabc));
    testCode.push_back(std::make_pair(0x124, 0x60000000));
    testCode.push_back(std::make_pair(0x150, 0xabc));
    testCode.push_back(std::make_pair(0x200, 0xabc));

    /* overwrite nop */
    REQUIRE_NOTHROW(Builder::addOriginalInstruction(testCode, args));
    REQUIRE(testCode[3].first == 0x124);
    REQUIRE(testCode[3].second == 0xe);

    /* should return error - instruction not nop */
    REQUIRE_THROWS_AS(Builder::addOriginalInstruction(testCode, args),
        std::runtime_error);
}

TEST_CASE("get asm code to inject")
{
    /* needed args */
    Arguments args;
    args.sources.push_back(path + "source/source1.c");
    args.sources.push_back(path + "source/source2.c");
    args.libs.push_back(path + "lib1.a");
    args.includePaths.push_back(path + "include");
    args.entry = "_main";
    args.injectAddress = 0x80377998;
    args.originalInstruction = 0x7ee3bb78;
    args.memRegions.push_back(MemRegion(0x803fa3e8, 0x803fc2ec));
    args.memRegions.push_back(MemRegion(0x803fc420, 0x803fdc1c));
    args.memRegions.push_back(MemRegion(0x801910e0, 0x80192930));
    args.memRegions.push_back(MemRegion(0x803001dc, 0x80301e40));

    /* compile source files, return object files */
    auto code = Builder::getASM(args);

    REQUIRE(code.size() == 244);

    REQUIRE(code[0].first == 0x803fa650);
    REQUIRE(code[0].second == 0x9421ffe8);

    REQUIRE(code[68].first == 0x80300244);
    REQUIRE(code[68].second == 0x4e800020);

    REQUIRE(code[69].first == 0x803fa714);
    REQUIRE(code[69].second == 0x41000000);

    REQUIRE(code[140].first == 0x803fa500);
    REQUIRE(code[140].second == 0x3920ffff);

    REQUIRE(code[180].first == 0x803fa5a0);
    REQUIRE(code[180].second == 0xc8080208);

    REQUIRE(code[240].first == 0x80377998);
    REQUIRE(code[240].second == 0x48082a50);

    REQUIRE(code[243].first == 0x803fa3f0);
    REQUIRE(code[243].second == 0x4bf7d5ac);

    Builder::cleanDirectory();
}
