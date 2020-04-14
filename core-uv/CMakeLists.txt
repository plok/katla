cmake_minimum_required(VERSION 3.10)
project(katla-core-uv)

include(../cmake/cmake-common.txt)
find_package(UV)
find_package(Absl)

set(target_name katla-core-uv)

include(../cmake/cmake-common.txt)

add_definitions(-DKATLA_CORE_UV_INDLL)

set(sources
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../external/outcome/
    ../external/abseil-cpp/
    ${LIBABSL_INCLUDE_DIR}
    ${LIBUV_INCLUDE_DIR}
    )

target_link_libraries(${target_name} ${LIBUV_LIBRARIES} fmt)
