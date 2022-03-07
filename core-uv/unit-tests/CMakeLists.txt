cmake_minimum_required(VERSION 3.10)
project(katla-core-uv-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

enable_testing()
include(GoogleTest)

enable_testing()

set(target_name katla-core-uv-tests)

set(sources
)

IF (NOT WIN32 AND NOT APPLE)
    set(sources ${sources}
        ../uv-core-application-test.cpp
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

target_link_libraries(${target_name} gtest gtest_main katla-core  katla-core-uv fmt)

gtest_discover_tests(${target_name})
