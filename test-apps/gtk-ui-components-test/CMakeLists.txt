cmake_minimum_required(VERSION 3.10)

set(target_name katla-gtk-ui-components-test)
project(${target_name})

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../../external/)

target_link_libraries(${target_name} katla-ui-components-gtk katla-ui-components katla-appkit)
