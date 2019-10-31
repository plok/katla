cmake_minimum_required(VERSION 3.10)
project(katla-gpu)

include(../cmake/cmake-common.txt)

add_definitions(-DKATLA_GPU_INDLL)

find_package(UV)

if(NOT WIN32) 
    include(FindPkgConfig)
    pkg_check_modules(GL REQUIRED gl)
    pkg_check_modules(glew REQUIRED glew)

    find_package(glfw3 3.2.0 REQUIRED)
endif()

if (SKIA_OPTION)
    include(${PROJECT_SOURCE_DIR}/../external/skia/skia-config.cmake)
endif()

set(SOURCES
    ../common/error.cpp

    
)

if (NOT WIN32)
    set(SOURCSE ${SOURCES}
        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/imgui_draw.cpp
        render/imgui/imgui_impl_glfw.cpp
        render/imgui/imgui_impl_opengl3.cpp
        render/imgui-glfw-binder.cpp
    )
endif()

if(VULKAN_OPTION)
    set(SOURCES ${SOURCES}
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
    )
endif()

if (OPENGL_OPTION)
    set(SOURCSE ${SOURCES}
        backend/opengl/opengl.cpp
        backend/opengl/opengl-window.cpp
        backend/opengl/opengl-window-factory.cpp
        backend/opengl/opengl-renderer-binders.cpp
    )
endif()

if (WIN32)
    set(SOURCES ${SOURCES}
        backend/direct2d/direct2d.cpp
        backend/direct2d/direct2d.h
        backend/direct2d/direct2d-surface.cpp
    )
endif()

if (SKIA_OPTION)
    set(SKIA_SOURCES render/skia-opengl-binder.cpp)
    set(SKIA_LIBS skia)
endif()

if (GLFW_OPTION)
    set(GLFW_LIBS glfw)
endif()

if (DIRECT2D_OPTION)
    set(DIRECT2D_LIBS D2d1)
endif()

add_library(katla-gpu SHARED ${SOURCES} ${SKIA_SOURCES})

target_include_directories(katla-gpu PRIVATE
    .
    ..
    ../external/
    ../external/imgui
    ../app-kit/
    ../gui-kit/
    )



target_link_libraries(katla-gpu katla-graphics katla-appkit katla-wm ${GLFW_LIBS} ${glew_LIBRARIES} ${GL_LIBRARIES} ${SKIA_LIBS} ${DIRECT2D_LIBS})

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
