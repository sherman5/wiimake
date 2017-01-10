# Core files that contain all functionality
set (CORE_SRCS  ArgumentParsing/CommandLineParser.cpp
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
                IsoHandling/ISO.cpp)     

set (CORE_HEADERS ArgumentParsing
                  HighLevel
                  LowLevel
                  IsoHandling)

# add library containing core files
add_library (core ${CORE_SRCS})

# get directories containing header files in core 
target_include_directories (core PUBLIC ${CORE_HEADERS})

# add command line programs
add_executable (wiimake MainProgram/wiimake.cpp)
add_executable (wiimake-ar MainProgram/wiimake-ar.cpp)
add_executable (wiimake-isotool MainProgram/wiimake-isotool.cpp)

# link against core library

target_link_libraries(wiimake core)
target_link_libraries(wiimake-ar core)
target_link_libraries(wiimake-isotool core)

# install targets
install (TARGETS wiimake DESTINATION ${CMAKE_INSTALL_PREFIX})
install (TARGETS wiimake-ar DESTINATION ${CMAKE_INSTALL_PREFIX})
install (TARGETS wiimake-isotool DESTINATION ${CMAKE_INSTALL_PREFIX})

# add unit tests
add_executable (testWiiMake ../tests/Runner.cpp
                            ../tests/testSystem.cpp
                            ../tests/testConfigFileParser.cpp
                            ../tests/testISO.cpp
                            ../tests/testCompiler.cpp
                            ../tests/testLinker.cpp
                            ../tests/testLinkerScript.cpp
                            ../tests/testObjectFile.cpp
                            ../tests/testSymbolTable.cpp  
                            ../tests/testMemory.cpp
                            ../tests/testCodeSections.cpp
                            ../tests/testBuilder.cpp)

target_link_libraries (testWiiMake core)
install (TARGETS testWiiMake DESTINATION ${CMAKE_BINARY_DIR})