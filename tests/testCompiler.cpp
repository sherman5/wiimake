#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

#include <iostream>

TEST_CASE("testing functions in Compiler.cpp")
{
    std::cout << "\033[33m" << 
    "\n========= Testing Compiler.cpp =========\n"
    << "\033[39m" << std::endl;

    FileList files;
    files.push_back("../tests/include");
    auto objects = Compiler::compile("../tests/source", files);
    REQUIRE(objects.size() == 2);
}
