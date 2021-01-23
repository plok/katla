cmake_minimum_required(VERSION 3.10)

set(target_name katla-gtk-application-test)
project(${target_name})

set(CMAKE_CXX_STANDARD 17)

set(sources
    main.cpp
)

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../../external/)

target_link_libraries(${target_name} katla-gtk katla-core)
