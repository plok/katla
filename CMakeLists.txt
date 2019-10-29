cmake_minimum_required(VERSION 3.10)
project(katla)

set(CMAKE_CXX_STANDARD 17)

if (NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(LIBUV REQUIRED libuv)
endif()

option(SKIA_OPTION "enable use of skia" FALSE)
option(VULKAN_OPTION "enable use of vulkan" TRUE)
option(OPENGL_OPTION "enable use of opengl" FALSE)

option(WAYLAND_OPTION "enable use of wayland" TRUE)

add_subdirectory(app-kit)
add_subdirectory(graphics)
add_subdirectory(window-management)

add_subdirectory(window-management-client-glfw)

add_subdirectory(gpu)

if (NOT WIN32)
    if (SKIA_OPTION)
        add_subdirectory(external/skia/skia-cmake)
    endif()
 
    add_subdirectory(graphics-freetype)

    if (VULKAN_OPTION)
        add_subdirectory(test-apps/vulkan-renderer-test)
    endif()

    if (OPENGL_OPTION)
        add_subdirectory(test-apps/opengl-renderer-test)
    endif()

    if (WAYLAND_OPTION)
        add_subdirectory(compositor/wayland)
        add_subdirectory(test-apps/wayland-test-client)
        add_subdirectory(test-apps/freetype-test)
    endif()
endif()

add_subdirectory(gui-kit)