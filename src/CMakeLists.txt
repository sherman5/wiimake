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
SET (GUI_SRCS GUI/MainWindow.cpp    
              GUI/PathInput.cpp
              GUI/Inject.cpp
              GUI/SaveLoad.cpp
              GUI/IsoInfo.cpp
              GUI/LibBuilder.cpp
              )

# Test files
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

# add unit tests
add_executable (testGCI ${TEST_SRCS} ${CORE_SRCS})
target_link_libraries (testGCI)

# add CLI
add_executable (GCI MainProgram/CLI.cpp
                    MainProgram/CommandLineParser.cpp
                    ${CORE_SRCS})

# install targets
install (TARGETS testGCI RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
install (TARGETS GCI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

# GUI
if (!BUILD_GUI)

    add_executable (GCI-gui MainProgram/GUI.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (GCI-gui Qt5::Widgets)
    install (TARGETS GCI-gui RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
endif()



