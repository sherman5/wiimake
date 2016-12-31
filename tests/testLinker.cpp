#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

#include <iostream>

TEST_CASE("testing functions in Linker.cpp")
{
    std::cout << "\033[33m" <<
    "\n========= Testing Linker.cpp =========\n"
    << "\033[39m" << std::endl;

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
