cmake_minimum_required(VERSION 3.10)
project(katla-core-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

enable_testing()

find_package(GTest)

set(target_name katla-core-tests)

set(sources
    ../posix-socket-test.cpp
)

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    ../../
    ../../external/
    ../../external/abseil-cpp/
    ../../external/outcome/
    ${GTEST_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} GTest::GTest GTest::Main katla-core fmt)

gtest_discover_tests(${target_name})
