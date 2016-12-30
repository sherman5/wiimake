#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../src/MainProgram/Parser.h"

TEST_CASE("testing functions in Memory.cpp")
{
    /* get arguments for testing */
    Arguments args;
    args.cmdOptions.insert(std::make_pair("--game-id", "2"));    
    args.cmdOptions.insert(std::make_pair("--config-file",
        "../tests/config.ini"));    
    
    /* get memory regions */    
    ConfigParser::parse(args);

    /* create section list */
    SectionList sections;
    sections.push_back(Section("section1"));
    sections.push_back(Section("section2"));
    sections.push_back(Section("section3"));
    sections.push_back(Section("section4"));
    sections.push_back(Section("section5"));
    sections.push_back(Section("section6"));
    sections.push_back(Section("section7"));
    
    /* store section sizes */
    sections[0].size = 0x800;
    sections[1].size = 0x1000;
    sections[2].size = 0x1500;
    sections[3].size = 0x1000;
    sections[4].size = 0x750;
    sections[5].size = 0x900;
    sections[6].size = 0x1500;

    SECTION("test finding allocation for code")
    {
       /* find allocation */
        Memory::findCodeAllocation(sections, args);

        /* check addresses - sections are sorted now */
        REQUIRE(sections[6].address == 0x803fa3f4);
        REQUIRE(sections[5].address == 0x803001dc);
        REQUIRE(sections[4].address == 0x801910e0);
        REQUIRE(sections[3].address == 0x803fc420);
        REQUIRE(sections[2].address == 0x803fb8f4);
        REQUIRE(sections[1].address == 0x801920e0);        
        REQUIRE(sections[0].address == 0x803fd420);
   }

    SECTION("test error handling")
    {
        sections[5].size = 0x1100;
        REQUIRE_THROWS_AS(Memory::findCodeAllocation(sections, args), 
            std::runtime_error);
    }        
}
