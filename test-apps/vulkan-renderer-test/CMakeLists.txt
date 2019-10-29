cmake_minimum_required(VERSION 3.10)
project(katla-vulkan-test-renderer)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)

find_package(glfw3 3.2.0 REQUIRED)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(katla-vulkan-test-renderer ${sources})

target_include_directories(katla-vulkan-test-renderer PRIVATE
    .
    ..
    ../../
    ../../external/)

target_link_libraries(katla-vulkan-test-renderer katla-gpu glfw katla-appkit)
