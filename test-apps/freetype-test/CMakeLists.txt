cmake_minimum_required(VERSION 3.10)
project(freetype-test)

set(CMAKE_CXX_STANDARD 17)

find_package(Freetype REQUIRED)

set(sources
    main.cpp
    ../../common/error.cpp
)

add_executable(freetype-test ${sources})

target_include_directories(freetype-test PRIVATE
    .
    ..
    ../../
    ../../gui-kit/
    ../../external/
    ${FREETYPE_INCLUDE_DIRS}
    )

target_link_libraries(freetype-test katla-appkit katla-compositor-wayland katla-graphics-freetype katla-graphics ${freetype2_LIBRARIES})
