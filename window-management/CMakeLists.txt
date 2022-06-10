cmake_minimum_required(VERSION 3.10)
project(katla-wm)

include(../cmake/cmake-common.txt)

add_definitions(-DKATLA_WINDOW_MANAGEMENT_INDLL)

set(sources
    window-factory.cpp
    window-factory.h
    window.cpp
    window.h)

add_library(katla-wm SHARED ${sources})

target_include_directories(katla-wm PRIVATE
    .
    ..
    ../../
    ../external/
    )

target_link_libraries(katla-wm katla-ui katla-core)

