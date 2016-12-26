# Core files that contain all functionality
SET (CORE_SRCS MainProgram/GCI.cpp
               HelperFunctions/Builder.cpp
               HelperFunctions/CodeSections.cpp
               HelperFunctions/Compiler.cpp
               HelperFunctions/ISO.cpp
               HelperFunctions/Linker.cpp
               HelperFunctions/LinkerScript.cpp
               HelperFunctions/Memory.cpp               
               HelperFunctions/ObjectFile.cpp
               HelperFunctions/SymbolTable.cpp
               HelperFunctions/SystemOperations.cpp 
               )

# build target for CLI
add_executable (GCI MainProgram/CLI.cpp
                    MainProgram/CommandLineParser.cpp
                    MainProgram/Description.cpp
                    ${CORE_SRCS}
                    )

# install target for CLI
install (TARGETS GCI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})


