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

    add_executable (GCI-CLI GCI-CLI.cpp ${CORE_SRCS})

endif ()

add_executable (GCI-ar GCI-ar.cpp ${CORE_SRCS})
add_executable (GCI-isotool GCI-isotool.cpp ${CORE_SRCS})

if (WIN32)
    target_link_libraries (GCI-isotool wsock32 ws2_32)
endif ()

if (WIN32)

    install (TARGETS GCI-CLI GCI-ar GCI-isotool RUNTIME DESTINATION "C:/Program Files/GCI" )

else ()

    install (TARGETS GCI-CLI GCI-ar GCI-isotool RUNTIME DESTINATION "/usr/local/bin/" )

endif ()

if (USE_QT)

    add_executable (GCI-GUI GCI-GUI.cpp ${CORE_SRCS} ${GUI_SRCS})
    target_link_libraries (GCI-GUI Qt5::Widgets)
    
    if (WIN32)

        install (TARGETS GCI-GUI RUNTIME DESTINATION "C:/Program Files/GCI")

    else ()

        install (TARGETS GCI-GUI RUNTIME DESTINATION "/usr/local/bin/")

    endif()

endif ()

