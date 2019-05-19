cmake_minimum_required(VERSION 3.10)
project(katla-graphics-freetype)

find_package(Freetype REQUIRED)

set(CMAKE_CXX_STANDARD 17)

SET(SOURCES 
    ../common/error.cpp
    text-rendering-freetype.cpp
    font-face-freetype.cpp
    font-face-surface-writer.cpp
    font-face-cache.cpp)

add_definitions(-DKATLA_GRAPHICS_FREETYPE_INDLL)

add_library(katla-graphics-freetype SHARED
    ${SOURCES} )

# add_custom_target(modules-katla-graphics-freetype
#    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build.py)

# add_dependencies(katla-graphics-freetype modules-katla-graphics-freetype)

# target_compile_options(katla-graphics-freetype
#   PRIVATE -fprebuilt-module-path=${CMAKE_BINARY_DIR}/graphics -fmodule-file=${CMAKE_BINARY_DIR}/graphics/image-file-writer.pcm)
  
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

target_link_libraries(katla-graphics-freetype INTERFACE katla-graphics Freetype::Freetype)
