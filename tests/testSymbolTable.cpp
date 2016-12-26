#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

SCENARIO ("Symbol Table parser works correctly")
{
    GIVEN("A single line of a symbol table")
    {
        /* read first line of file */
        std::ifstream file = std::ifstream("symbolTable.txt");
        std::string line;
        file >> line;

        /* vector of sizes */
        std::vector<unsigned> sizes;
        sizes.reserve(7);

        SymbolTable::parseLine(file, line, sizes);  

        REQUIRE(sizes.empty());
    }
}

