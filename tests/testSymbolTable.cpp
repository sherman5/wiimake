#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

static const std::string path = "../tests/files/SymbolTable/";

TEST_CASE("parse whole table")
{
    auto sz = SymbolTable::getSizes(path + "symbolTable.out", 7);
    /*REQUIRE(sz[0] == 16);
    REQUIRE(sz[1] == 20);
    REQUIRE(sz[2] == 160);
    REQUIRE(sz[3] == 16);
    REQUIRE(sz[4] == 44);
    REQUIRE(sz[5] == 4);
    REQUIRE(sz[6] == 12);*/
}

