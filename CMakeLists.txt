# minimum version for finding boost (?)
cmake_minimum_required (VERSION 3.1.0)

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

# add directory with source files
add_subdirectory (src)


