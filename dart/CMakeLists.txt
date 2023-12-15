cmake_minimum_required(VERSION 3.10)
project(katla-dart)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

set(target_name katla-dart)

add_definitions(-DKATLA_DART_INDLL)

set(sources
    dart-api/dart_api_dl.c
    dart-api/dart_api_dl.h
    dart-api/dart_native_api.h

    c-api/dart-image.cpp
    c-api/dart-image.h
    )

add_library(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../external/
    ${OpenCV_INCLUDE_DIRS}
    )

target_link_libraries(${target_name}
    fmt
    katla-core
    katla-image
    opencv_core
    opencv_imgcodecs)

target_compile_options(${target_name} PRIVATE ${KATLA_INTERN_WARNING_OPTIONS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name})

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()
