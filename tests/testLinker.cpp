#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "../src/HighLevel/HighLevel.h"
#include "HeaderDisplay.h"

static const std::string prefix = "../tests/files/Linker/";

TEST_CASE("test linker")
{
    /* display header in first test case */
    displayHeader("Testing Linker.cpp");

    /* create sections */
    SectionList sections;
    sections.push_back(Section(prefix + "object1.o", 0x80003000));
    sections.push_back(Section(prefix + "object2.o", 0x80004000));
    sections.push_back(Section(prefix + "lib1.a", 0x80005000));

    /* create script */ 
    LinkerScript::CreateFinalScript(sections, prefix + "linkScript.txt");

}
