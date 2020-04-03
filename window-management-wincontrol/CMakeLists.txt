cmake_minimum_required(VERSION 3.10)
project(katla-window-management-wincontrol)

include(../cmake/cmake-common.txt)

find_package(UV)

add_definitions(-DKATLA_WINDOW_MANAGEMENT_WINCONTROL_INDLL)

set(target_name katla-window-management-wincontrol)

set(sources
    ../common/error.cpp
    
    wincontrol-window.cpp
    wincontrol-window-factory.cpp
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../external/
    ../
    ../app-kit/
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} katla-wm katla-graphics katla-gpu)
