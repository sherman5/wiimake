#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE("create sample scripts")
{
    /* display header in first test case */
    displayHeader("Testing LinkeScript.cpp");

    /* create section list */
    SectionList sections;
    sections.push_back(Section("section1 (data)"));
    sections.push_back(Section("section2 (data)"));
    sections.push_back(Section("section3 (data)"));
    sections.push_back(Section("section4 (data)"));
    sections.push_back(Section("section5"));
    sections.push_back(Section("section6"));
    sections.push_back(Section("section7"));
    sections.push_back(Section("section8"));

    /* create script to record sizes */
    LinkerScript::CreateTempScript(sections, "../tests/script1.txt");

    /* add addresses to sections */
    sections[0].address = 0x100;
    sections[1].address = 0x200;
    sections[2].address = 0x300;
    sections[3].address = 0x400;
    sections[4].address = 0x500;
    sections[5].address = 0x600;
    sections[6].address = 0x700;
    sections[7].address = 0x800;    
    
    /* create final linker script with addresses */
    LinkerScript::CreateFinalScript(sections, "../tests/script2.txt");
}
