cmake_minimum_required(VERSION 3.10)
project(katla-websockets)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

find_package(Threads REQUIRED)

set(target_name katla-websockets)

add_definitions(-DKATLA_WEBSOCKETS_INDLL)

set(sources
        incoming-http-request.cpp
        incoming-http-request.h
        http-request.h
        websocket-server-lws.cpp
        websocket-server-lws.h
        websocket-server-lws-private.cpp
        websocket-server-lws-private.h
        websocket-server-client-lws.cpp
        websocket-server-client-lws.h
        websocket-server-client-lws-impl.cpp
        websocket-server-client-lws-impl.h
        )

add_library(${target_name} ${sources})

target_include_directories(${target_name} PUBLIC 
        ../../
        ../external/   # TODO make suitable for FindPackage
        ../external/cxxopt/
        ${LIBGSL_INCLUDE_DIR}
        )

target_include_directories(${target_name} PRIVATE
        .
        )

target_link_libraries(${target_name} katla-core fmt websockets_shared)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${target_name} Threads::Threads)

    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
        target_link_libraries(${target_name} stdc++fs)
    endif ()
endif ()
