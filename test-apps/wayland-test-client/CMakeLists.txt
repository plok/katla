cmake_minimum_required(VERSION 3.10)
project(wayland-test-client)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)

pkg_check_modules(wayland-client REQUIRED wayland-client)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(wayland-test-client ${sources})

target_include_directories(wayland-test-client PRIVATE
    .
    ..
    ../../
    ../../gui-kit/
    ../../external/)

target_link_libraries(wayland-test-client katla-appkit katla-compositor-wayland)
