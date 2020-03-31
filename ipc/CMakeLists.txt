cmake_minimum_required(VERSION 3.10)
project(katla-ipc)

include(../cmake/cmake-common.txt)
find_package(UV)

set(target_name katla-ipc)

include(../cmake/cmake-common.txt)

add_definitions(-DKATLA_IPCKIT_INDLL)

set(sources
    ../common/error.cpp
    ipc-kit.cpp
    ipc-kit.h
    process-uv.cpp

    frame-reader.cpp
    pipe.cpp

    flatbuffer/katla-ipc_generated.h
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../external/outcome/
    ../external/abseil-cpp/
    ../app-kit/
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} ${LIBUV_LIBRARIES} fmt)
