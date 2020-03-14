cmake_minimum_required(VERSION 3.10)
project(katla)

set(CMAKE_CXX_STANDARD 17)

if (NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(LIBUV REQUIRED libuv)
endif()

option(SKIA_OPTION "enable use of skia" FALSE)

option(GLFW_OPTION "enable use of glfw" TRUE)
option(QT_OPTION "enable use of qt" FALSE)

option(VULKAN_OPTION "enable use of vulkan" TRUE)
option(OPENGL_OPTION "enable use of opengl" FALSE)
option(DIRECT2D_OPTION "enable use of Direct2D" FALSE)

option(WAYLAND_OPTION "enable use of wayland" TRUE)

add_subdirectory(app-kit)
add_subdirectory(graphics)
add_subdirectory(window-management)

add_subdirectory(ui-components)

if (QT_OPTION)
    add_subdirectory(ui-components-qt)
    add_subdirectory(test-apps/qt-ui-components-test)
endif()

if (GLFW_OPTION)
    add_subdirectory(window-management-client-glfw)
endif()

add_subdirectory(gpu)

add_subdirectory(ipc)
add_subdirectory(ipc/tests)
add_subdirectory(test-apps/ipc-test)


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

if (WIN32)
    add_subdirectory(window-management-wincontrol)
endif()

add_subdirectory(gui-kit)

if (WIN32)
    add_subdirectory(test-apps/win32-gui-test)
endif()
