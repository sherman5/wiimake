#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"

TEST_CASE("testing functions in CodeSections.cpp")
{
    /* create file list */
    FileList files;
    files.push_back("../tests/object1.o");
    files.push_back("../tests/object2.o");
    files.push_back("../tests/lib1.a");

    /* get sections */
    SectionList sections;
    REQUIRE_NOTHROW(CodeSections::storeNames(sections, files));

    SECTION("test storing section names")
    {
        REQUIRE(sections.size() == 11);        
        REQUIRE(sections[0].path == "../tests/object2.o (text)");
        REQUIRE(sections[1].path == "../tests/object2.o (attr)");
        REQUIRE(sections[2].path == "../tests/lib1.a (text0)");
        REQUIRE(sections[3].path == "../tests/lib1.a (attr0)");
        REQUIRE(sections[4].path == "../tests/lib1.a (text2)");
        REQUIRE(sections[5].path == "../tests/lib1.a (rodata2)");
        REQUIRE(sections[6].path == "../tests/lib1.a (attr2)");
        REQUIRE(sections[7].path == "../tests/lib1.a (text1)");
        REQUIRE(sections[8].path == "../tests/lib1.a (text2)");
        REQUIRE(sections[9].path == "../tests/lib1.a (rodata2)");
        REQUIRE(sections[10].path == "../tests/lib1.a (attr2)");
    }

    SECTION("test storing section sizes")
    {
        REQUIRE_NOTHROW(CodeSections::storeSizes(sections));
        REQUIRE(sections.size() == 11);
        REQUIRE(sections[0].size == 0x2C);
        REQUIRE(sections[1].size == 0x10);
        REQUIRE(sections[2].size == 0);
        REQUIRE(sections[3].size == 0);
        REQUIRE(sections[4].size == 0);
        REQUIRE(sections[5].size == 0);
        REQUIRE(sections[6].size == 0);
        REQUIRE(sections[7].size == 0);
        REQUIRE(sections[8].size == 0);
        REQUIRE(sections[9].size == 0);
        REQUIRE(sections[10].size == 0);
    }
}
