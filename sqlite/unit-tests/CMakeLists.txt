cmake_minimum_required(VERSION 3.10)
project(katla-sqlite-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

enable_testing()
include(GoogleTest)

enable_testing()

set(target_name katla-sqlite-tests)

set(sources
    ../sqlite-database-tests.cpp
)

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    ../../
    ../../../
    ../../external/
    ../../external/outcome/
    ${GTEST_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} gtest gtest_main katla-sqlite fmt)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name})

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()


gtest_discover_tests(${target_name})
