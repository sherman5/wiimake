#include "catch.hpp"

#include "../src/HelperFunctions/HelperFunctions.h"

#include <fstream>

TEST_CASE("testing functions in System.cpp")
{
    SECTION("test running a command")
    {
        /* make sure temp files do not exist */
        /*System::runCMD(System::rm + " temp.out");
        System::runCMD(System::rm + " temp1.txt");
        std::ifstream f1("temp.out");
        std::ifstream f2("temp1.txt");
        REQUIRE(!f1.good());
        REQUIRE(!f2.good());
        f1.close();
        f2.close();

        /* run long command to make sure next command waits */        
        /*System::runCMD("powerpc-eabi-gcc ../tests/longCompile.c -o"
            " temp.out");
        System::runCMD("powerpc-eabi-objdump -t temp.out > temp1.txt");

        /* make sure output file exists */
        /*std::ifstream f3("temp1.txt");
        REQUIRE(f3.good());
        f3.close();

        /* remove temp files */
        /*System::runCMD(System::rm + " temp.out");  
        System::runCMD(System::rm + " temp1.txt");   
    }

    SECTION("test getting all files in a directory")
    {
        /* get files */
        FileList files = System::getFiles("../tests/", "cpp");

        REQUIRE(files.size() == 12);
    }
}
