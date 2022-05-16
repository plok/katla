cmake_minimum_required(VERSION 3.10)
project(katla-core-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

enable_testing()
include(GoogleTest)

enable_testing()

set(target_name katla-core-tests)

set(sources
)

IF (NOT WIN32 AND NOT APPLE)
    set(sources ${sources}
        ../posix-socket-test.cpp
        ../string-utils-test.cpp
    )
ENDIF()

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    ../../
    ../../external/
    ../../external/abseil-cpp/
    ../../external/outcome/
    ${GTEST_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} gtest gtest_main katla-core fmt)

gtest_discover_tests(${target_name})
