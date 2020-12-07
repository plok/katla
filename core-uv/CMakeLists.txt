cmake_minimum_required(VERSION 3.10)
project(katla-core-uv)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

find_package(Threads REQUIRED)
find_package(Uv REQUIRED)

set(target_name katla-core-uv)

add_definitions(-DKATLA_CORE_UV_INDLL)

set(sources
    uv-event-loop.cpp
    uv-event-loop.h
    uv-signal-handler.cpp
    uv-signal-handler.h
    uv-timer.cpp
    uv-timer.h
    uv-core-application.cpp
    uv-core-application.h
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../external/
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} ${LIBUV_LIBRARIES} fmt)
