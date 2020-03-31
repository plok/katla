cmake_minimum_required(VERSION 3.10)
project(katla-compositor-wayland)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(wayland-client REQUIRED wayland-client)

set(sources
    ../../common/error.cpp
    client/wayland-display.cpp
    client/wayland-shm-pool.cpp
    client/wayland-buffer.cpp
    client/wayland-surface.cpp
    client/wayland-xdg-shell-surface.cpp
    api/xdg-shell-client-protocol-unstable-v6.c
)

add_library(katla-compositor-wayland SHARED ${sources})

target_include_directories(katla-compositor-wayland PRIVATE
    .
    ..
    ../../
)

target_link_libraries(katla-compositor-wayland katla-graphics katla-appkit wayland-client rt)
