cmake_minimum_required(VERSION 3.10)
project(katla-sqlite)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

find_package(Threads REQUIRED)
find_package(Uv REQUIRED)
find_package(SQLite3 REQUIRED)

set(target_name katla-sqlite)

add_definitions(-DKATLA_SQLITE_INDLL)

set(sources
    sqlite-errors.cpp 
    sqlite-errors.h
    sqlite-database.cpp
    sqlite-database.h
    )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../external/
    ${SQLite3_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} fmt katla-core SQLite::SQLite3)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name})

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()
