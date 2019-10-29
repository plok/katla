cmake_minimum_required(VERSION 3.10)
project(katla-graphics-freetype)

find_package(Freetype REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DKATLA_GRAPHICS_FREETYPE_INDLL)

set(sources
    ../common/error.cpp

    text-renderer-freetype.cpp
    font-face-freetype.cpp
    font-face-writer.cpp
    font-face-cache.cpp
    )

add_library(katla-graphics-freetype SHARED ${sources})

target_include_directories(katla-graphics-freetype PRIVATE
    .
    ..
    ../external/
    ../external/imgui
    ../app-kit/
    ../gui-kit/
    ../graphics/
    ${FREETYPE_INCLUDE_DIRS}
    )

target_link_libraries(katla-graphics-freetype katla-graphics ${FREETYPE_LIBRARIES})
