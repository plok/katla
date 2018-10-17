cmake_minimum_required(VERSION 3.10)
project(katla-gpu)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(glew REQUIRED glew)

find_package(glfw3 3.2.0 REQUIRED)

set (skia_path ${PROJECT_SOURCE_DIR}/../external/skia)

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
    ../common/error.cpp
    
    render/skia-opengl-binder.cpp

        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/imgui_draw.cpp
        render/imgui/imgui_impl_glfw.cpp
        render/imgui/imgui_impl_opengl3.cpp
        render/imgui-glfw-binder.cpp
        ../../../imgui-scene.cpp


    backend/vulkan/vulkan.cpp
    backend/vulkan/vulkan-window.cpp
    backend/vulkan/vulkan-window-factory.cpp
    backend/vulkan/vulkan-physical-device.cpp
    backend/vulkan/vulkan-physical-device-factory.cpp
    backend/vulkan/vulkan-device.cpp
    backend/vulkan/vulkan-device-factory.cpp
    backend/vulkan/vulkan-device-queue.cpp

    backend/opengl/opengl.cpp
    backend/opengl/opengl-window.cpp
    backend/opengl/opengl-window-factory.cpp
    backend/opengl/opengl-renderer-binders.cpp
    render/render-view.cpp 
    render/render-stats.cpp)

add_library(katla-gpu SHARED ${sources})

target_include_directories(katla-gpu PRIVATE
    .
    ..
    ../external/
        ../external/imgui
    ../app-kit/
    ../gui-kit/
    ${skia_include_directories})

target_link_libraries(katla-gpu glfw ${glew_LIBRARIES} ${GL_LIBRARIES} ${skia} )
