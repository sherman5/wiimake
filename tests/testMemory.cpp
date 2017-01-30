#include "catch.hpp"

#include "../src/HighLevel/HighLevel.h"
#include "HeaderDisplay.h"

TEST_CASE("find code allocation")
{
    /* display header in first test case */
    displayHeader("Testing Memory.cpp");

    /* get arguments for testing */
    Arguments args;
    args.fixedSymbols.push_back(FixedSymbol("", 0, 0));
    args.fixedSymbols.push_back(FixedSymbol("", 0, 0));
    args.memRegions.push_back(MemRegion(0x803fa3e8, 0x803fc2ec));
    args.memRegions.push_back(MemRegion(0x803fc420, 0x803fdc1c));
    args.memRegions.push_back(MemRegion(0x801910e0, 0x80192930));
    args.memRegions.push_back(MemRegion(0x803001dc, 0x80301e40));

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

    /* find allocation */
    Memory::findCodeAllocation(sections, args);

    /* check addresses */
    REQUIRE(sections[0].address == 0x801920e4);                
    REQUIRE(sections[1].address == 0x803fc420);
    REQUIRE(sections[2].address == 0x803001f4);
    REQUIRE(sections[3].address == 0x801910e0);
    REQUIRE(sections[4].address == 0x803fd424);
    REQUIRE(sections[5].address == 0x803fb8ec);    
    REQUIRE(sections[6].address == 0x803fa3e8);
    
    /* should not be able to find allocation now */
    sections[5].size = 0x1100;
    REQUIRE_THROWS_AS(Memory::findCodeAllocation(sections, args), 
        std::runtime_error);
}
