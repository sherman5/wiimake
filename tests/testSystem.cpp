#include "catch.hpp"

#include "../src/LowLevel/LowLevel.h"
#include "HeaderDisplay.h"

#include <fstream>

static const std::string path = "../tests/files/System/";

TEST_CASE("running commands")
{
    /* display header in first test case */
    displayHeader("Testing System.cpp");

    /* make sure temp files do not exist */
    System::runCMD(System::rm + path + "testFile.out");
    System::runCMD(System::rm + path + "testFile.txt");
    std::ifstream f1(path + "testFile.out");
    std::ifstream f2(path + "testFile.txt");
    REQUIRE(!f1.good());
    REQUIRE(!f2.good());
    f1.close();
    f2.close();

    /* run long command to make sure next command waits */        
    System::runCMD("powerpc-eabi-gcc " + path + "longCompile.c -o "
        + path + "testFile.out");
    System::runCMD("powerpc-eabi-objdump -t " + path + "testFile.out > "
        + path + "testFile.txt");

    /* make sure output file exists */
    std::ifstream f3(path + "testFile.txt");
    REQUIRE(f3.peek() != std::ifstream::traits_type::eof());
    f3.close();

    /* remove test files */
    System::runCMD(System::rm + " " + path + "testFile.out");  
    System::runCMD(System::rm + " " + path + "testFile.txt");
}



