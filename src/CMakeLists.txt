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

# add test, install to current directory
add_executable (testGCI ${TEST_SRCS} ${CORE_SRCS})
install (TARGETS testGCI RUNTIME DESTINATION ${CMAKE_BINARY_DIR})

# get libraries needed for windows
if (WIN32)

    set (EXTRA_LIBS wsock32 ws2_32)

endif()

# add CLI
add_executable (GCI MainProgram/CLI.cpp
                    MainProgram/CommandLineParser.cpp
                    ${CORE_SRCS})

target_link_libraries (GCI ${EXTRA_LIBS})

# add GUI
if (BUILD_GUI)

    add_executable (GCI-gui MainProgram/GUI.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (GCI-gui Qt5::Widgets ${EXTRA_LIBS})

endif()

# install targets
install (TARGETS GCI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install (TARGETS GCI-gui RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

