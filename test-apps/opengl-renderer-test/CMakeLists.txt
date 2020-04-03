cmake_minimum_required(VERSION 3.10)
project(katla-opengl-test-renderer)

include(../../cmake/cmake-common.txt)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(katla-opengl-test-renderer ${sources})

target_include_directories(katla-opengl-test-renderer PRIVATE
    .
    ..
    ../../
    ../../external/
    ${skia_include_directories})

target_link_libraries(katla-opengl-test-renderer katla-gpu katla-graphics katla-wm glfw katla-appkit ${GL_LIBRARIES})
