# Core files that contain all functionality
SET (CORE_SRCS ArgumentParsing/CommandLineParser.cpp
               ArgumentParsing/ConfigFileParser.cpp
               HighLevel/Builder.cpp
               HighLevel/CodeSections.cpp
               HighLevel/LinkerScript.cpp
               HighLevel/Memory.cpp
               LowLevel/Compiler.cpp
               LowLevel/Linker.cpp
               LowLevel/ObjectFile.cpp
               LowLevel/SymbolTable.cpp
               LowLevel/System.cpp
               IsoHandling/ISO.cpp
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
               )

# add unit tests
add_executable (testWiiMake ${TEST_SRCS} ${CORE_SRCS})

# add command line programs
add_executable (wiimake MainProgram/wiimake.cpp ${CORE_SRCS})
add_executable (wiimake-ar MainProgram/wiimake-ar.cpp ${CORE_SRCS})
add_executable (wiimake-isotool MainProgram/wiimake-isotool.cpp ${CORE_SRCS})

# install targets
install (TARGETS testWiiMake RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
install (TARGETS wiimake RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install (TARGETS wiimake-ar RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install (TARGETS wiimake-isotool RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

# GUI
if (BUILD_GUI)

    add_executable (wiimake-gui MainProgram/wiimake-gui.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (wiimake-gui Qt5::Widgets)
    install (TARGETS wiimake-gui RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
endif()



