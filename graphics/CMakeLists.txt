cmake_minimum_required(VERSION 3.10)
project(katla-graphics)

include(../cmake/cmake-common.txt)

add_definitions(-DKATLA_GRAPHICS_INDLL)

set(sources
    ../common/error.cpp

    graphics-backend.cpp

    render-view.cpp 
    render-stats.cpp
    image.cpp
    image-processor.cpp
    simple-painter.cpp
    )

add_library(katla-graphics SHARED ${sources})

target_include_directories(katla-graphics PRIVATE
    .
    ..
    ../external/
    ../external/imgui
    ../app-kit/
    ../gui-kit/
    )

target_link_libraries(katla-graphics)
