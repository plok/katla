cmake_minimum_required(VERSION 3.10)
project(win32-gui-test)

include(../../cmake/cmake-common.txt)

SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)
SET (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)

find_package(UV)

set(target_name win32-gui-test)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(${target_name} WIN32 ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../../gui-kit/
    ../../external/
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} katla-appkit katla-wm katla-window-management-wincontrol katla-guikit)
