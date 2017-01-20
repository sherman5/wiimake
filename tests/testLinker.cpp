#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "../src/HighLevel/HighLevel.h"
#include "HeaderDisplay.h"

static const std::string path = "../tests/files/Linker/";

TEST_CASE("linker success")
{
    /* display header in first test case */
    displayHeader("Testing Linker.cpp");

    /* create sections */
    SectionList sections;
    sections.push_back(Section(path + "object1.o", 0x80003000));
    sections.push_back(Section(path + "object2.o", 0x80004000));
    sections.push_back(Section(path + "lib1.a", 0x80005000));

    /* create list of object files */
    FileList objects = {path + "object1.o", path + "object2.o",
        path + "lib1.a"};

    /* create script */ 
    LinkerScript::CreateFinalScript(sections, path + "linkScript.txt");

    /* link files */
    REQUIRE_NOTHROW(Linker::link(objects, path + "linkScript.txt",
        "linked.out", "_main"));
}

TEST_CASE("linker failure")
{
    /* create sections */
    SectionList sections;
    sections.push_back(Section(path + "object1.o", 0x80003000));
    sections.push_back(Section(path + "object2.o", 0x80004000));

    /* create list of object files */
    FileList objects = {path + "object1.o", path + "object2.o"};

    /* create script */ 
    LinkerScript::CreateFinalScript(sections, path + "linkScript.txt");

    /* link files */
    REQUIRE_THROWS_AS(Linker::link(objects, path + "linkScript.txt",
        "linked.out", "_main"), std::runtime_error);

}

