#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"
#include "../tests/HeaderDisplay.h"

#include <fstream>

TEST_CASE("running commands")
{
    /* display header in first test case */
    displayHeader("Testing System.cpp");

    /* make sure temp files do not exist */
    System::runCMD(System::rm + " testFile.out");
    System::runCMD(System::rm + " testFile.txt");
    std::ifstream f1("testFile.out");
    std::ifstream f2("testFile.txt");
    REQUIRE(!f1.good());
    REQUIRE(!f2.good());
    f1.close();
    f2.close();

    /* run long command to make sure next command waits */        
    System::runCMD("powerpc-eabi-gcc ../tests/longCompile.c -o"
        " testFile.out");
    System::runCMD("powerpc-eabi-objdump -t testFile.out > testFile.txt");

    /* make sure output file exists */
    std::ifstream f3("testFile.txt");
    REQUIRE(f3.good());
    f3.close();

    /* remove test files */
    System::runCMD(System::rm + " testFile.out");  
    System::runCMD(System::rm + " testFile.txt");
}

TEST_CASE("get list of files from directory")
{
    FileList files = System::getFiles("../tests/", "cpp");
    REQUIRE(files.size() == 14);
}



