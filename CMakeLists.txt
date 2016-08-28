cmake_minimum_required (VERSION 2.6)

# set compiler flags
set (CMAKE_CXX_FLAGS "-std=c++11" ${CMAKE_CXX_FLAGS})

# project name
project (GCI)

# set up boost if on windows
find_package (Boost 1.46.0 COMPONENTS program_options)

if (WIN32 AND NOT Boost_FOUND )
    
    message(FATAL_ERROR "error: boost/program_options is required")

elseif (WIN32)

    set (USE_BOOST TRUE)
    
else ()

    set (USE_BOOST FALSE)

endif()

# add directory with source files
add_subdirectory (src)


