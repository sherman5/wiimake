# Core files that contain all functionality
SET (CORE_SRCS MainProgram/GCI.cpp
               MainProgram/ConfigFileParser.cpp
               HelperFunctions/Builder.cpp
               HelperFunctions/CodeSections.cpp
               HelperFunctions/Compiler.cpp
               HelperFunctions/ISO.cpp
               HelperFunctions/Linker.cpp
               HelperFunctions/LinkerScript.cpp
               HelperFunctions/Memory.cpp               
               HelperFunctions/ObjectFile.cpp
               HelperFunctions/SymbolTable.cpp
               HelperFunctions/System.cpp 
               )

# Files used for GUI
SET (GUI_SRCS GUI/CustomObjects.cpp
              GUI/Inject.cpp
              GUI/Isotool.cpp
              GUI/LibBuilder.cpp
              GUI/MainWindow.cpp    
              GUI/MemConfig.cpp
              GUI/SaveLoad.cpp
              )

# sources for testing
SET (TEST_SRCS ../tests/Runner.cpp
               ../tests/testSystem.cpp
               ../tests/testConfigFileParser.cpp
               ../tests/testCommandLineParser.cpp
               ../tests/testISO.cpp
               ../tests/testCompiler.cpp
               ../tests/testLinker.cpp
               ../tests/testLinkerScript.cpp
               ../tests/testObjectFile.cpp
               ../tests/testSymbolTable.cpp  
               ../tests/testMemory.cpp
               ../tests/testCodeSections.cpp
               ../tests/testBuilder.cpp
               ../tests/testGCI.cpp
               MainProgram/CommandLineParser.cpp
               )

# add test, install to current directory
add_executable (testGCI ${TEST_SRCS} ${CORE_SRCS})
install (TARGETS testGCI RUNTIME DESTINATION ${CMAKE_BINARY_DIR})

# add CLI
add_executable (GCI MainProgram/CLI.cpp
                    MainProgram/CommandLineParser.cpp
                    ${CORE_SRCS})

# install target for CLI
 install (TARGETS GCI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

# add GUI
