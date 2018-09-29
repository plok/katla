cmake_minimum_required(VERSION 3.10)
project(katla-appkit)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)

set(sources
    ../common/error.cpp

    core-application.cpp
    event-loop-uv.cpp
    timer-uv.cpp
    signal-handler-uv.cpp
)

add_library(katla-appkit SHARED ${sources})

target_include_directories(katla-appkit PRIVATE
    .
    ..
    ../external/
    ${skia_include_directories})

target_link_libraries(katla-appkit ${LIBUV_LIBRARIES})
