# Core files that contain all functionality
SET (CORE_SRCS GCI.cpp
               CodeAssembler.cpp
               FileOperations.cpp
               ISOhandler.cpp
               ObjdumpFileParser.cpp
               MemoryConfig.cpp)

# files that create and manage the GUI
SET (GUI_SRCS GUI_MainWindow.cpp
              GUI_MemConfig.cpp
              GUI_SaveLoad.cpp
              GUI_LibBuilder.cpp
              GUI_Isotool.cpp
              GUI_Inject.cpp
              GUI_CustomObjects.cpp)

# GUI
if (BUILD_GUI)
    
    # Same source files for all platforms
    add_executable (GCI-GUI GCI-GUI.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (GCI-GUI Qt5::Widgets)

    # Windows
    if (WIN32) 

        # need libraries for searching the file system
        target_link_libraries (GCI-GUI wsock32 ws2_32)

    endif()

    # install target for GUI
    install (TARGETS GCI-GUI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
endif()    

# CLI
if (BUILD_CLI)

    # Windows
    if (WIN32)

        # include boost headers
        include_directories (SYSTEM ${Boost_INCLUDE_DIR})

        # windows CLI source files
        add_executable (GCI-CLI GCI-CLI_boost.cpp ${CORE_SRCS})

        # link against boost
        target_link_libraries (GCI-CLI ${Boost_LIBRARIES} wsock32 ws2_32)

    # Linux
    else()

        # linux CLI source files
        add_executable (GCI-CLI GCI-CLI_argp.cpp ${CORE_SRCS})

    endif()

    # install target for CLI
    install (TARGETS GCI-CLI RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

endif()


