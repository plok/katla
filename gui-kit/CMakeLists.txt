cmake_minimum_required(VERSION 3.10)
project(katla-guikit)

include(../cmake/cmake-common.txt)

find_package(UV)

add_definitions(-DKATLA_GUIKIT_INDLL)

set(sources
    ../common/error.cpp

    gui-kit.cpp
)

add_library(katla-guikit SHARED ${sources})

target_include_directories(katla-guikit PRIVATE
    .
    ..
    ${LIBUV_INCLUDE_DIR}
    ../external/)

target_link_libraries(katla-guikit katla-appkit katla-graphics ${LIBUV_LIBRARIES})
