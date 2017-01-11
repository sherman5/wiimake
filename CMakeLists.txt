# minimum version for finding boost (?)
cmake_minimum_required (VERSION 3.1.0)

# set compiler flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic" CACHE STRING "g++ flags")

# link statically
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# project name
project (WiiMake)

# project version
set (WiiMake_VERSION_MAJOR 1)
set (WiiMake_VERSION_MINOR 0)
set (WiiMake_FIX_NUMBER 2)

# create header for version number
configure_file (
    "src/MainProgram/Version.h.in"
    "${PROJECT_BINARY_DIR}/Version.h")

# add header to include path
include_directories ("${PROJECT_BINARY_DIR}")

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

set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${WiiMake_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${WiiMake_VERSION_MINOR}")
include (CPack)


