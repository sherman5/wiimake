#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE ("parsing single line")
{
    /* display header in first test case */
    displayHeader("Testing SymbolTable.cpp");

    /* vector of sizes */
    std::vector<unsigned> sizes;
    sizes.resize(7);

    /* open up text file with symbol table */
    std::ifstream file = std::ifstream("../tests/symbolTable.txt");
    std::string line;

    /* expect error since not at first column of table */
    REQUIRE_THROWS(SymbolTable::parseLine(file, line, sizes));
    
    /* get to first column of table */
    while (line != "0:")
    {
        file >> line;
    }   

    /* parse first 12 lines */
    for (unsigned i = 0; i < 12; ++i)
    {
        REQUIRE_NOTHROW(SymbolTable::parseLine(file, line, sizes));
    }

    /* should find no sizes */
    for (unsigned i = 0; i < 7; ++i)
    {
        REQUIRE(sizes[i] == 0);
    }

    /* section 7 has size 12 */
    SymbolTable::parseLine(file, line, sizes);
    REQUIRE(sizes[6] == 12);

    /* section 3 has size 160 */
    SymbolTable::parseLine(file, line, sizes);
    REQUIRE(sizes[2] == 160);

    /* section 1 has size 16 */
    SymbolTable::parseLine(file, line, sizes);
    REQUIRE(sizes[0] == 16);
}

TEST_CASE("parse whole table")
{
    auto sz = SymbolTable::getSizes("../tests/symbolTable.out", 7);
    REQUIRE(sz[0] == 16);
    REQUIRE(sz[1] == 20);
    REQUIRE(sz[2] == 160);
    REQUIRE(sz[3] == 16);
    REQUIRE(sz[4] == 44);
    REQUIRE(sz[5] == 4);
    REQUIRE(sz[6] == 12);
}

