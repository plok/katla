cmake_minimum_required(VERSION 3.10)
project(katla-core-uv)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

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
    uv-posix-process.cpp
    uv-posix-process.h
    )

add_library(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../external/
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} uv fmt)

target_compile_options(${target_name} PRIVATE ${KATLA_INTERN_WARNING_OPTIONS})

