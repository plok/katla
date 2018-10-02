cmake_minimum_required(VERSION 3.10)
project(katla-guikit)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)

set(sources
    ../common/error.cpp

    gui-kit.cpp
)

set (skia_path ${PROJECT_SOURCE_DIR}/../external/skia)

set (skia_output_path ${skia_path}/out/Shared/)
set (skia_include_base_path ${skia_path}/include)

find_library(skia skia ${skia_output_path})

set (skia_include_directories
    ${skia_include_base_path}
    ${skia_include_base_path}/core
    ${skia_include_base_path}/config
    ${skia_include_base_path}/gpu
    ${skia_include_base_path}/utils
    ${skia_include_base_path}/private
)

add_library(katla-guikit SHARED ${sources})

target_include_directories(katla-guikit PRIVATE
    .
    ..
    ../external/
    ${skia_include_directories})

target_link_libraries(katla-guikit katla-appkit ${LIBUV_LIBRARIES})
