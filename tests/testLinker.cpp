#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

TEST_CASE("testing functions in Linker.cpp")
{
    /* create sections */
    SectionList sections;
    sections.push_back(Section("../tests/object1.o", 0x80003000));
    sections.push_back(Section("../tests/object2.o", 0x80004000));
    sections.push_back(Section("../tests/lib1.a", 0x80005000));

    /* create script */ 
    LinkerScript::CreateFinalScript(sections, "../tests/linkScript.txt");

    SECTION("test linking files")
    {
        Linker::link(sections, "../tests/linkScript.txt",
            "../tests/linked.out");
    }
}
