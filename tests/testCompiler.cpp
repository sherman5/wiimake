#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../tests/HeaderDisplay.h"

TEST_CASE("compile files in directory")
{
    /* display header in first test case */
    displayHeader("Testing Compiler.cpp");

    FileList files;
    files.push_back("../tests/include");
    auto objects = Compiler::compile("../tests/source", files);
    REQUIRE(objects.size() == 2);
}
