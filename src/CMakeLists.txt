if (USE_BOOST) 
    
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
    add_executable (GCI GCI_boost.cpp CodeAssembler.cpp FileOperations.cpp ISOhandler.cpp ObjdumpFileParser.cpp RegionFileParser.cpp)
    target_link_libraries (GCI ${Boost_LIBRARIES} wsock32 ws2_32)

else ()

    add_executable (GCI GCI.cpp CodeAssembler.cpp FileOperations.cpp ISOhandler.cpp ObjdumpFileParser.cpp RegionFileParser.cpp)

endif ()

add_executable (GCI-ar GCI-ar.cpp FileOperations.cpp ObjdumpFileParser.cpp RegionFileParser.cpp)

add_executable (GCI-isotool GCI-isotool.cpp ISOhandler.cpp ObjdumpFileParser.cpp RegionFileParser.cpp)
if (WIN32)
    target_link_libraries (GCI-isotool wsock32 ws2_32)
endif ()

if (WIN32)

    install (TARGETS GCI GCI-ar GCI-isotool RUNTIME DESTINATION "C:/Program Files/GCI" )

else ()

    install (TARGETS GCI GCI-ar GCI-isotool RUNTIME DESTINATION "/usr/local/bin/" )

endif ()
