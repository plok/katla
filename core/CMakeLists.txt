cmake_minimum_required(VERSION 3.10)
project(katla-core)

include(../cmake/cmake-common.txt)
find_package(Absl)

set(target_name katla-core)

include(../cmake/cmake-common.txt)

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
    ../external/abseil-cpp/
    ${LIBABSL_INCLUDE_DIR}
    )

target_link_libraries(${target_name} fmt)
