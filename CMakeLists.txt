cmake_minimum_required(VERSION 3.10)
project(katla)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)

add_subdirectory(external/skia/skia-cmake)
add_subdirectory(app-kit)
add_subdirectory(gpu)
add_subdirectory(gpu/test-apps/renderer-test)
add_subdirectory(gui-kit)