cmake_minimum_required(VERSION 3.10)
project(katla-renderer-test)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)

set (skia_path ../../skia)
set (skia_output_path ${skia_path}/out/Shared/)
set (skia_include_base_path ${skia_path}/include)

find_library(skia skia ${skia_output_path})

set (skia_include_directories
    ${skia_include_base_path}
    ${skia_include_base_path}/core
    ${skia_include_base_path}/config
    ${skia_include_base_path}/gpu
    ${skia_include_base_path}/utils
    ${skia_include_base_path}/private
)

set(sources
    main.cpp
    ../common/error.cpp
    drawing/skia-opengl.cpp
    gpu/vulkan/vulkan.cpp
    gpu/vulkan/vulkan-window.cpp
    gpu/vulkan/vulkan-window-factory.cpp
    gpu/vulkan/vulkan-physical-device.cpp
    gpu/vulkan/vulkan-physical-device-factory.cpp
    gpu/vulkan/vulkan-device.cpp
    gpu/vulkan/vulkan-device-factory.cpp
    gpu/vulkan/vulkan-device-queue.cpp

    gpu/opengl/opengl.cpp
    gpu/opengl/opengl-window.cpp
    gpu/opengl/opengl-window-factory.cpp
)

add_executable(katla-renderer ${sources})

target_include_directories(katla-renderer PRIVATE
    .
    ..
    ../external/
    ${skia_include_directories})

target_link_libraries(katla-renderer glfw ${GL_LIBRARIES} ${skia})
