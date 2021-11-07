cmake_minimum_required(VERSION 3.10)
project(katla-ui-components-gtk)

include(../cmake/cmake-common.txt)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

add_definitions(-DKATLA_UI_COMPONENTS_QT_INDLL)

set(target_name katla-ui-components-gtk)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(sources
    ../common/error.cpp
    ui-components-gtk.cpp
   )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PUBLIC
    ${GTK3_INCLUDE_DIRS}
    .
    ..
    ../external/
    ../
    ../app-kit/
    )

target_link_directories(${target_name} PUBLIC
    ${GTK3_LIBRARY_DIRS}
)

target_link_libraries(${target_name} katla-wm ${GTK3_LIBRARIES})
