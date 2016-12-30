#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

TEST_CASE("testing functions in Compiler.cpp")
{
    SECTION("test compiler")
    {
        FileList files;
        files.push_back("../tests/include");
        auto objects = Compiler::compile("../tests/", files);
        REQUIRE(objects.size() == 3);
    }
}
