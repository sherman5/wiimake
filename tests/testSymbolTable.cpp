#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

TEST_CASE ("Testing functions in SymbolTable.cpp")
{
    SECTION("parsing a single line of a symbol table (from text file)")
    {
        /* vector of sizes */
        std::vector<unsigned> sizes;
        sizes.resize(7);

        /* read first line of file */
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
    
    SECTION("parse entire object file (from object file)")
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
}

