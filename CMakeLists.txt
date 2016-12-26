# minimum version for finding boost (?)
cmake_minimum_required (VERSION 2.8.11)

# set compiler flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11"
        CACHE STRING "g++ flags")

# project name
project (GCI)

# set default install directories
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

    # windows install directory
    if (WIN32)

        set (CMAKE_INSTALL_PREFIX "C:/Program Files/GCI"
                CACHE PATH "default install path")

    # linux install directory
    else ()

        set (CMAKE_INSTALL_PREFIX "/usr/local/bin"
                CACHE PATH "default install path" FORCE)

    endif()

endif()

# build CLI by default, GUI only if QT is available
set (BUILD_CLI TRUE)

# look for qt (don't build GUI if qt not available)
find_package (Qt5Widgets QUIET)

# if QT is installed
if (Qt5Widgets_FOUND)

    # Whether to handle moc automatically for Qt targets
    set (CMAKE_AUTOMOC ON)

    # build GUI since QT was found
    set (BUILD_GUI TRUE)

# if QT is not installed
else()

    # don't build GUI
    set (BUILD_GUI FALSE)
    message(WARNING "QT not found: GUI will not be built")

endif()

# allow for tests to be added 
enable_testing(true)

# add directory with source files
add_subdirectory (src)


