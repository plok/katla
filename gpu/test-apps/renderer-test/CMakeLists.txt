cmake_minimum_required(VERSION 3.10)
project(katla-renderer-test)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)

set(sources
    main.cpp
    ../../../common/error.cpp
)

add_executable(katla-renderer ${sources})

target_include_directories(katla-renderer PRIVATE
    .
    ..
    ../../../
    ../../../external/
    ${skia_include_directories})

target_link_libraries(katla-renderer katla-gpu glfw skia katla-appkit ${GL_LIBRARIES})
