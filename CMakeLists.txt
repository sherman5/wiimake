# minimum version for finding boost (?)
cmake_minimum_required (VERSION 3.1.0)

# set compiler flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic"
        CACHE STRING "g++ flags")

# project name
project (wiimake)

# set default install directories
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

    # windows install directory
    if (WIN32)

        set (CMAKE_INSTALL_PREFIX "C:/Program Files/wiimake"
                CACHE PATH "default install path")

    # linux install directory
    else ()

        set (CMAKE_INSTALL_PREFIX "/usr/local/bin"
                CACHE PATH "default install path" FORCE)

    endif()

endif()

# add directory with source files
add_subdirectory (src)


