cmake_minimum_required(VERSION 3.10)
project(katla-image-tests)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-common.txt)

enable_testing()
include(GoogleTest)

enable_testing()

set(target_name katla-image-tests)

set(sources
)

IF (NOT WIN32 AND NOT APPLE)
    set(sources ${sources}
        ../c-api/image-test.cpp
    )
ENDIF()

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    ../../
    ../../external/
    ${GTEST_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    )

target_link_libraries(${target_name}
    gtest
    gtest_main
    katla-core
    katla-image
    fmt opencv_core
    opencv_imgcodecs
    /home/simon/projects/adjustcentroids/build/linux/x64/debug/bundle/lib/libflutter_linux_gtk.so)

gtest_discover_tests(${target_name})
