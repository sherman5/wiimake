SET (CORE_SRCS GCI.cpp
               CodeAssembler.cpp
               FileOperations.cpp
               ISOhandler.cpp
               ObjdumpFileParser.cpp
               MemoryConfig.cpp)

SET (GUI_SRCS GUI_MainWindow.cpp
              GUI_MemConfig.cpp
              GUI_SaveLoad.cpp
              GUI_LibBuilder.cpp
              GUI_Isotool.cpp
              GUI_Inject.cpp
              GUI_CustomObjects.cpp)

if (USE_BOOST) 
    
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
    add_executable (GCI-CLI GCI-CLI_boost.cpp ${CORE_SRCS})
    target_link_libraries (GCI ${Boost_LIBRARIES} wsock32 ws2_32)

else ()

    add_executable (GCI-CLI GCI-CLI_argp.cpp ${CORE_SRCS})

endif ()

if (USE_QT)

    add_executable (GCI-GUI GCI-GUI.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (GCI-GUI Qt5::Widgets)

endif ()

if (WIN32)

    target_link_libraries (GCI-CLI wsock32 ws2_32)
    target_link_libraries (GCI-GUI wsock32 ws2_32)

endif()

if (WIN32)

    if (USE_QT)

        install (TARGETS GCI-GUI RUNTIME DESTINATION "C:/Program Files/GCI")

    endif()        

    install (TARGETS GCI-CLI RUNTIME DESTINATION "C:/Program Files/GCI" )

else ()

    if (USE_QT)

        install (TARGETS GCI-GUI RUNTIME DESTINATION "/usr/local/bin/")

    endif()

    install (TARGETS GCI-CLI RUNTIME DESTINATION "/usr/local/bin/" )

endif()
