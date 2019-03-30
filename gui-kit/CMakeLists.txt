cmake_minimum_required(VERSION 3.10)
project(katla-guikit)

set(CMAKE_CXX_STANDARD 17)

find_library(LIBUV_LIBRARIES NAMES uv)

set(sources
    ../common/error.cpp

    gui-kit.cpp
)

add_library(katla-guikit SHARED ${sources})

target_include_directories(katla-guikit PRIVATE
    .
    ..
    ../external/)

target_link_libraries(katla-guikit katla-appkit katla-graphics ${LIBUV_LIBRARIES})
