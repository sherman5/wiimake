#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"

#include <iostream>

TEST_CASE("testing functions in LinkerScript.cpp")
{
    std::cout << "\033[33m" <<
    "\n========= Testing LinkerScript.cpp =========\n"
    << "\033[39m" << std::endl;

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

    /* get arguments for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));    
    
    /* get memory regions */    
    ConfigParser::parse(args);

    /* add sizes and addresses to sections */
    sections[0].size = 0x800;
    sections[1].size = 0x1000;
    sections[2].size = 0x1500;
    sections[3].size = 0x1000;
    sections[4].size = 0x750;
    sections[5].size = 0x900;
    sections[6].size = 0x1500;
    sections[7].size = 0;    
    Memory::findCodeAllocation(sections, args);
    
    /* create final linker script with addresses */
    LinkerScript::CreateFinalScript(sections, "../tests/script2.txt");
}
