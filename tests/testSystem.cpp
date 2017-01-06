#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

#include <fstream>

TEST_CASE("running commands")
{
    /* display header in first test case */
    displayHeader("Testing System.cpp");

    /* make sure temp files do not exist */
    System::runCMD(System::rm + " ../tests/files/System/testFile.out");
    System::runCMD(System::rm + " ../tests/files/System/testFile.txt");
    std::ifstream f1("../tests/files/System/testFile.out");
    std::ifstream f2("../tests/files/System/testFile.txt");
    REQUIRE(!f1.good());
    REQUIRE(!f2.good());
    f1.close();
    f2.close();

    /* run long command to make sure next command waits */        
    System::runCMD("powerpc-eabi-gcc ../tests/files/System/longCompile.c -o"
        " ../tests/files/System/testFile.out");
    System::runCMD("powerpc-eabi-objdump -t "
    "../tests/files/System/testFile.out > "
    "../tests/files/System/testFile.txt");

    /* make sure output file exists */
    std::ifstream f3("../tests/files/System/testFile.txt");
    REQUIRE(f3.peek() != std::ifstream::traits_type::eof());
    f3.close();

    /* remove test files */
    System::runCMD(System::rm + " ../tests/files/System/testFile.out");  
    System::runCMD(System::rm + " ../tests/files/System/testFile.txt");
}



