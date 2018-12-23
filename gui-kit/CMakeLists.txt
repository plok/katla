cmake_minimum_required(VERSION 3.10)
project(katla-guikit)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)

set(sources
    ../common/error.cpp

    gui-kit.cpp
)

add_library(katla-guikit SHARED ${sources})

target_include_directories(katla-guikit PRIVATE
    .
    ..
    ../external/
    ${skia_include_directories})

target_link_libraries(katla-guikit katla-appkit skia ${LIBUV_LIBRARIES})
