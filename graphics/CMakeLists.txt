cmake_minimum_required(VERSION 3.10)
project(katla-graphics)

find_package(Freetype REQUIRED)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fmodules-ts)

add_definitions(-DKATLA_GRAPHICS_INDLL)

#set(sources
#    ../common/error.cpp
#
#    render-view.cpp 
#    render-stats.cpp
#    image.cpp
#    image-drawing-simple.cpp
#    )

#set(modules
#    ${CMAKE_BINARY_DIR}/image-file-writer.o)

set(KATLA_GRAPHICS_INCLUDE_DIRECTORIES
    .
    ../
    ../external/
    ../external/imgui
    ../app-kit/
    ../gui-kit/
    ${FREETYPE_INCLUDE_DIRS}
    )

add_library(katla-graphics SHARED IMPORTED)

message (${CMAKE_BINARY_DIR})

add_custom_target(modules-katla-graphics
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build.py)

add_dependencies(katla-graphics modules-katla-graphics)

# target_compile_options(katla-graphics
#  PRIVATE -fprebuilt-module-path=${CMAKE_BINARY_DIR}/graphics)

#target_include_directories(katla-graphics PRIVATE
#    .
#    ..
#    ../external/
#    ../external/imgui
#    ../app-kit/
#    ../gui-kit/
#    ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(katla-graphics ${freetype2_LIBRARIES})
