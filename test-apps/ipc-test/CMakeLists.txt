cmake_minimum_required(VERSION 3.10)
project(ipc-test)

include(../../cmake/cmake-common.txt)

set(target_name katla-ipc-test)

set(sources
    main.cpp
    ../../common/error.cpp
)

set(child_sources
    main-child.cpp
    ../../common/error.cpp
)

add_executable(${target_name} ${sources})
add_executable(${target_name}-child ${child_sources})

set(${target_name}_include_dirs
    .
    ..
    ../../
    ../../../
    ../../../external/
)

target_include_directories(${target_name} PRIVATE ${target_name}_include_dirs)
target_include_directories(${target_name}-child PRIVATE ${target_name}_include_dirs)

target_link_libraries(${target_name} katla-appkit katla-ipc)
target_link_libraries(${target_name}-child katla-appkit katla-ipc)
