#include "catch.hpp"

#include "../src/HighLevel/HighLevel.h"
#include "../src/ArgumentParsing/Arguments.h"
#include "HeaderDisplay.h"

static const std::string path = "../tests/files/CodeSections/";

TEST_CASE("get section names and sizes")
{
    /* display header in first test case */
    displayHeader("Testing CodeSections.cpp");

    /* create file list */
    FileList files;
    files.push_back(path + "object1.o");
    files.push_back(path + "object2.o");
    files.push_back(path + "lib1.a");

    /* get section names */
    SectionList sections;
    sections.push_back(Section(path + "inject_point_0.o", 0x80377998));
    sections.push_back(Section(path + "inject_point_1.o", 0x80371fe4));
    sections.push_back(Section(path + "stack_setup_0.o"));
    sections.push_back(Section(path + "stack_setup_1.o"));

    REQUIRE_NOTHROW(CodeSections::storeNames(sections, files));

    REQUIRE(sections.size() == 17);        
    REQUIRE(sections[0].path == path + "inject_point_0.o");
    REQUIRE(sections[1].path == path + "inject_point_1.o");
    REQUIRE(sections[2].path == path + "stack_setup_0.o");
    REQUIRE(sections[3].path == path + "stack_setup_1.o");
    REQUIRE(sections[4].path == path + "object1.o (text)");
    REQUIRE(sections[5].path == path + "object1.o (rodata)");
    REQUIRE(sections[6].path == path + "object1.o (attr)");
    REQUIRE(sections[7].path == path + "object2.o (text)");
    REQUIRE(sections[8].path == path + "object2.o (attr)");
    REQUIRE(sections[9].path == path + "lib1.a (text0)");
    REQUIRE(sections[10].path == path + "lib1.a (attr0)");
    REQUIRE(sections[11].path == path + "lib1.a (text1)");
    REQUIRE(sections[12].path == path + "lib1.a (rodata1)");
    REQUIRE(sections[13].path == path + "lib1.a (attr1)");
    REQUIRE(sections[14].path == path + "lib1.a (text2)");
    REQUIRE(sections[15].path == path + "lib1.a (rodata2)");
    REQUIRE(sections[16].path == path + "lib1.a (attr2)");

    /* get section sizes */
    Arguments args;
    args.fixedSymbols.push_back(FixedSymbol("", 0, 0));
    args.fixedSymbols.push_back(FixedSymbol("", 0, 0));
    REQUIRE_NOTHROW(CodeSections::storeSizes(sections, args));

    REQUIRE(sections.size() == 17);
    REQUIRE(sections[0].size == 0x4);
    REQUIRE(sections[1].size == 0x4);
    REQUIRE(sections[2].size == 0x54);
    REQUIRE(sections[3].size == 0x54);
    REQUIRE(sections[4].size == 0xc0);
    REQUIRE(sections[5].size == 0x14);
    REQUIRE(sections[6].size == 0x10);
    REQUIRE(sections[7].size == 0x2c);
    REQUIRE(sections[8].size == 0x10);
    REQUIRE(sections[9].size == 0);
    REQUIRE(sections[10].size == 0);
    REQUIRE(sections[11].size == 0x258);
    REQUIRE(sections[12].size == 0x3c);
    REQUIRE(sections[13].size == 0x10);
    REQUIRE(sections[14].size == 0x130);
    REQUIRE(sections[15].size == 0xc);
    REQUIRE(sections[16].size == 0x10);
}

