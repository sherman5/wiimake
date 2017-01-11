#include "catch.hpp"

#include "../src/HighLevel/HighLevel.h"
#include "../src/ArgumentParsing/Parser.h"
#include "HeaderDisplay.h"

#include <algorithm>
#include <string>

static const std::string prefix = "../tests/files/Builder/";

/* ignore back/forward slashes in windows */
#ifdef GCI_WINDOWS

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

TEST_CASE("create library")
{
    /* display header in first test case */
    displayHeader("Testing Builder.cpp");

    /* needed args */
    Arguments args;
    args.name = prefix + "libtest.a";
    args.sources.push_back(prefix + "lib/Controller.c");
    args.sources.push_back(prefix + "lib/Math.c");
    args.sources.push_back(prefix + "lib/Random.c");

    /* remove old library */
    System::runCMD(System::rm + " " + prefix + "libtest.a");
    
    /* create library */
    REQUIRE_NOTHROW(Builder::buildLibrary(args));

    /* get sections */
    auto libSections = ObjectFile::getNamedSections(prefix + "libtest.a");

    /* check section names */
    REQUIRE(libSections.size() == 8);
    REQUIRE(libSections[0] == "text0");
    REQUIRE(libSections[1] == "attr0");
    REQUIRE(libSections[2] == "text1");
    REQUIRE(libSections[3] == "rodata1");
    REQUIRE(libSections[4] == "attr1");
    REQUIRE(libSections[5] == "text2");
    REQUIRE(libSections[6] == "rodata2");
    REQUIRE(libSections[7] == "attr2");
}

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

TEST_CASE("allocate sections given object files")
{
    /* needed args */
    Arguments args;
    args.sources.push_back(prefix + "source/source1.c");
    args.sources.push_back(prefix + "source/source2.c");
    args.libs.push_back(prefix + "lib1.a");
    args.includePaths.push_back(prefix + "include");
    args.entry = "_main";
    args.injectAddress = 0x80377998;
    args.originalInstruction = 0x7ee3bb78;
    args.memRegions.push_back(MemRegion(0x803fa3e8, 0x803fc2ec));
    args.memRegions.push_back(MemRegion(0x803fc420, 0x803fdc1c));
    args.memRegions.push_back(MemRegion(0x801910e0, 0x80192930));
    args.memRegions.push_back(MemRegion(0x803001dc, 0x80301e40));

    /* compile source files, return object files */
    auto objects = Builder::getObjectFiles(args.sources, TokenList(), 
        args.includePaths, args.libs);
    
    REQUIRE(objects.size() == 3);    
    REQUIRE(STR_EQ(objects[0], prefix + "source/source1.o"));    
    REQUIRE(STR_EQ(objects[1], prefix + "source/source2.o"));
    REQUIRE(STR_EQ(objects[2], prefix + "lib1.a"));

    /* find addresses for each section */
    auto sections = Builder::getSectionAddresses(objects, args);

    REQUIRE(sections.size() == 13);

    REQUIRE(STR_EQ(sections[0].path, prefix + "source/source1.o (text)"));
    REQUIRE(STR_EQ(sections[1].path, prefix + "source/source1.o (rodata)"));
    REQUIRE(STR_EQ(sections[2].path, prefix + "source/source1.o (attr)"));
    REQUIRE(STR_EQ(sections[3].path, prefix + "source/source2.o (text)"));
    REQUIRE(STR_EQ(sections[4].path, prefix + "source/source2.o (attr)"));
    REQUIRE(STR_EQ(sections[5].path, prefix + "lib1.a (text0)"));
    REQUIRE(STR_EQ(sections[6].path, prefix + "lib1.a (attr0)"));
    REQUIRE(STR_EQ(sections[7].path, prefix + "lib1.a (text1)"));
    REQUIRE(STR_EQ(sections[8].path, prefix + "lib1.a (rodata1)"));
    REQUIRE(STR_EQ(sections[9].path, prefix + "lib1.a (attr1)"));
    REQUIRE(STR_EQ(sections[10].path, prefix + "lib1.a (text2)"));
    REQUIRE(STR_EQ(sections[11].path, prefix + "lib1.a (rodata2)"));
    REQUIRE(STR_EQ(sections[12].path, prefix + "lib1.a (attr2)"));

    REQUIRE(sections[0].size == 0xc0);
    REQUIRE(sections[1].size == 0x14);
    REQUIRE(sections[2].size == 0x10);    
    REQUIRE(sections[3].size == 0x2c);
    REQUIRE(sections[4].size == 0x10);        
    REQUIRE(sections[5].size == 0x0);
    REQUIRE(sections[6].size == 0x0);
    REQUIRE(sections[7].size == 0x258);
    REQUIRE(sections[8].size == 0x3c);
    REQUIRE(sections[9].size == 0x10);
    REQUIRE(sections[10].size == 0x0);
    REQUIRE(sections[11].size == 0x0);
    REQUIRE(sections[12].size == 0x0);

    REQUIRE(sections[0].address == 0x803fa650);
    REQUIRE(sections[1].address == 0x8030024c);    
    REQUIRE(sections[2].address == 0x80300278);
    REQUIRE(sections[3].address == 0x8030021c);
    REQUIRE(sections[4].address == 0x803fa714);            
    REQUIRE(sections[5].address == 0x0);
    REQUIRE(sections[6].address == 0x0);
    REQUIRE(sections[7].address == 0x803fa3f4);        
    REQUIRE(sections[8].address == 0x803001dc); 
    REQUIRE(sections[9].address == 0x80300264);       
    REQUIRE(sections[10].address == 0x0);
    REQUIRE(sections[11].address == 0x0);
    REQUIRE(sections[12].address == 0x0);

    /* add stack setup files */
    Builder::addStackSetup(sections, args);

    REQUIRE(sections.size() == 15);

    REQUIRE(sections[13].path == "stack_setup.o");
    REQUIRE(sections[13].address == 0x803fa3e8);

    REQUIRE(sections[14].path == "inject_point.o");
    REQUIRE(sections[14].address == 0x80377998);
}

TEST_CASE("link sections and extract code")
{
    /* needed args */
    Arguments args;
    args.sources.push_back(prefix + "source/source1.c");
    args.sources.push_back(prefix + "source/source2.c");
    args.libs.push_back(prefix + "lib1.a");
    args.includePaths.push_back(prefix + "include");
    args.injectAddress = 0x80377998;
    args.originalInstruction = 0x7ee3bb78;
    args.entry = "_main";
    args.memRegions.push_back(MemRegion(0x803fa3e8, 0x803fc2ec));
    args.memRegions.push_back(MemRegion(0x803fc420, 0x803fdc1c));
    args.memRegions.push_back(MemRegion(0x801910e0, 0x80192930));
    args.memRegions.push_back(MemRegion(0x803001dc, 0x80301e40));

    /* compile source files, return object files */
    auto objects = Builder::getObjectFiles(args.sources, TokenList(),
        args.includePaths, args.libs);

    /* find addresses for each section */
    auto sections = Builder::getSectionAddresses(objects, args);

    /* add stack setup files */
    Builder::addStackSetup(sections, args);

    /* link all code in sections */
    System::runCMD(System::rm + " final.out");
    ASMcode code = Builder::getLinkedCode(sections, args);

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

    REQUIRE(code[243].first == 0x80377998);
    REQUIRE(code[243].second == 0x48082a50);

    Builder::cleanDirectory();
}
