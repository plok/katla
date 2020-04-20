cmake_minimum_required(VERSION 3.10)
project(katla-core)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

find_package(Threads REQUIRED)
find_package(Absl REQUIRED)
find_package(Fmt REQUIRED)

set(target_name katla-core)

add_definitions(-DKATLA_CORE_INDLL)

set(sources
    posix-errors.cpp
    posix-pipe.cpp
    posix-socket.cpp
    posix-file.cpp
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../external/outcome/
    ${LIBABSL_INCLUDE_DIR}
    ${LIBFMT_INCLUDE_DIR}
    )

target_link_libraries(${target_name} fmt)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name} Threads::Threads)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()
