#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

TEST_CASE("compile files in directory")
{
    /* display header in first test case */
    displayHeader("Testing Compiler.cpp");

    FileList sources, includes;
    sources.push_back("../tests/files/source/source1.c");
    sources.push_back("../tests/files/source/source2.c");
    includes.push_back("../tests/include");
    auto objects = Compiler::compile(sources, includes);
    REQUIRE(objects.size() == 2);
}
