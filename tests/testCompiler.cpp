#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

TEST_CASE("compile files in directory")
{
    /* display header in first test case */
    displayHeader("Testing Compiler.cpp");

    FileList sources, includes;
    sources.push_back("../tests/files/Compiler/source1.c");
    sources.push_back("../tests/files/Compiler/source2.c");
    includes.push_back("../tests/files/Compiler/include");
    auto objects = Compiler::compileAll(sources, TokenList(), includes);
    REQUIRE(objects.size() == 2);
    REQUIRE(objects[0] == "../tests/files/Compiler/source1.o");
    REQUIRE(objects[1] == "../tests/files/Compiler/source2.o");
}
