cmake_minimum_required(VERSION 3.10)
project(katla-image)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

set(target_name katla-image)

add_definitions(-DKATLA_IMAGE_INDLL)

set(sources
    image.cpp 
    image.h 
    image-processor.cpp 
    image-processor.h
    lut.cpp
    lut.h
    opencv-image-processor.cpp 
    opencv-image-processor.h
    opencv-image-reader.cpp 
    opencv-image-reader.h
    opencv-image-writer.cpp
    opencv-image-writer.h

    c-api/image.cpp
    c-api/image.h
    )

add_library(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../dart/
    ../../
    ../external/
    ${OpenCV_INCLUDE_DIRS}
    )

target_link_libraries(${target_name}
    fmt
    katla-core
    opencv_core
    opencv_imgcodecs)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name})

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()
