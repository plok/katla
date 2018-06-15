cmake_minimum_required(VERSION 3.10)
project(katla)

set(CMAKE_CXX_STANDARD 11)
set(KMSXX_ENABLE_PYTHON false)
add_subdirectory(external/kmsxx)

add_executable(katla main.cpp src/compositor/Compositor.cpp src/compositor/Compositor.h)

target_link_libraries(katla vulkan kms++ kms++util)