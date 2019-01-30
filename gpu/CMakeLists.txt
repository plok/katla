cmake_minimum_required(VERSION 3.10)
project(katla-gpu)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(glew REQUIRED glew)

find_package(glfw3 3.2.0 REQUIRED)

include(${PROJECT_SOURCE_DIR}/../external/skia/skia-config.cmake)

set(sources
    ../common/error.cpp
    
    render/skia-opengl-binder.cpp

    ../external/imgui/imgui.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui_draw.cpp
    render/imgui/imgui_impl_glfw.cpp
    render/imgui/imgui_impl_opengl3.cpp
    render/imgui-glfw-binder.cpp

    backend/vulkan/vulkan.cpp
    backend/vulkan/vulkan-window.cpp
    backend/vulkan/vulkan-window-factory.cpp
    backend/vulkan/vulkan-physical-device.cpp
    backend/vulkan/vulkan-physical-device-factory.cpp
    backend/vulkan/vulkan-device.cpp
    backend/vulkan/vulkan-device-factory.cpp
    backend/vulkan/vulkan-device-queue.cpp
    backend/vulkan/vulkan-graphics-pipeline.cpp
    backend/vulkan/vulkan-render-pass.cpp
    backend/vulkan/vulkan-shader.cpp
    backend/vulkan/vulkan-framebuffer.cpp
    backend/vulkan/vulkan-engine.cpp
    backend/vulkan/vulkan-command-pool.cpp
    backend/vulkan/vulkan-semaphore.cpp

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
    )

target_link_libraries(katla-gpu glfw ${glew_LIBRARIES} ${GL_LIBRARIES} skia)

add_custom_command(TARGET katla-gpu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_BINARY_DIR}/shaders/)

add_custom_command(TARGET katla-gpu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
            ${CMAKE_BINARY_DIR}/shaders/vert.spv)

add_custom_command(TARGET katla-gpu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
            ${CMAKE_BINARY_DIR}/shaders/frag.spv)
