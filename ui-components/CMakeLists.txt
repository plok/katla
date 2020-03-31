cmake_minimum_required(VERSION 3.10)
project(katla-ui-components)

include(../cmake/cmake-common.txt)

find_package(UV)

add_definitions(-DKATLA_UI_COMPONENTS_INDLL)

set(target_name katla-ui-components)

set(sources
    ../common/error.cpp
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

target_link_libraries(${target_name})
