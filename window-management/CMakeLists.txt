cmake_minimum_required(VERSION 3.10)
project(katla-wm)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DKATLA_WINDOW_MANAGEMENT_INDLL)

set(sources
    ../common/error.cpp)

add_library(katla-wm SHARED ${sources})

target_include_directories(katla-wm PRIVATE
    .
    ..
    ../external/
    ../external/imgui
    ../app-kit/
    ../gui-kit/
    )

target_link_libraries(katla-wm)

