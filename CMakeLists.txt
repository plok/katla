cmake_minimum_required(VERSION 3.10)
project(katla)

include(cmake/cmake-common.txt)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(external/googletest)

add_subdirectory(core)
add_subdirectory(core/unit-tests)