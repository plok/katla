cmake_minimum_required(VERSION 3.10)
project(katla-websockets-integration-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

set(target_name ${PROJECT_NAME})

set(sources
        main.cpp
        )

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ../../../
    ../../../external/cxxopt/
    ../../../external/outcome/
    ${LIBFMT_INCLUDE_DIR}
    )

target_link_libraries(${target_name} katla-core katla-core-uv katla-websockets)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        TARGET_LINK_LIBRARIES (${target_name} pthread)

        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
                TARGET_LINK_LIBRARIES (${target_name} stdc++fs)
        endif()
endif()

