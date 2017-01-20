#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

static const std::string path = "../tests/files/Compiler/";

TEST_CASE("compile single file")
{
    /* display header in first test case */
    displayHeader("Testing Compiler.cpp");

    std::string object;
    REQUIRE_NOTHROW(object = Compiler::compile(path + "source1.c",
        TokenList(), FileList(1, path + "include/")));

    REQUIRE(object == path + "source1.o");
}

TEST_CASE("test compiler failure")
{
    //REQUIRE_THROWS_AS(Compiler::compile(path + "error.c"),
      //  std::runtime_error);
}

TEST_CASE("compile files in directory")
{
    FileList sources;

    sources.push_back(path + "source1.c");
    sources.push_back(path + "source2.c");

    FileList objects;
    REQUIRE_NOTHROW(objects = Compiler::compileAll(sources,
        TokenList(1, "-std=c99"), FileList(1, path + "include/")));

    REQUIRE(objects.size() == 2);
    REQUIRE(objects[0] == path + "source1.o");
    REQUIRE(objects[1] == path + "source2.o");
}
