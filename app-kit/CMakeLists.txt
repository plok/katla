cmake_minimum_required(VERSION 3.10)
project(katla-appkit)

include(../cmake/cmake-common.txt)

find_package(UV)

add_definitions(-DKATLA_APPKIT_INDLL)

set(sources
    ../common/error.cpp

    core-application-uv.cpp
    event-loop-uv.cpp
    timer-uv.cpp
    signal-handler-uv.cpp
)

add_library(katla-appkit SHARED ${sources})

message(${LIBUV_INCLUDE_DIR})

target_include_directories(katla-appkit PUBLIC
    .
    ..
    ../external/
    ${LIBUV_INCLUDE_DIR}
    ${skia_include_directories})

message(${LIBUV_INCLUDE_DIR})

target_link_libraries(katla-appkit ${LIBUV_LIBRARIES})
