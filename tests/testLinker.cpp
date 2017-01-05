#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

TEST_CASE("test linker")
{
    /* display header in first test case */
    displayHeader("Testing Linker.cpp");

    /* create sections */
    SectionList sections;
    sections.push_back(Section("../tests/object1.o", 0x80003000));
    sections.push_back(Section("../tests/object2.o", 0x80004000));
    sections.push_back(Section("../tests/lib1.a", 0x80005000));

    /* create script */ 
    LinkerScript::CreateFinalScript(sections, "../tests/linkScript.txt");

    /* link code */
    Linker::link(sections, "../tests/linkScript.txt",
        "../tests/linked.out");
}
