cmake_minimum_required(VERSION 3.10)
project(katla-ui-components-qt)

include(../cmake/cmake-common.txt)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_definitions(-DKATLA_UI_COMPONENTS_QT_INDLL)

set(target_name katla-ui-components-qt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(sources
    ../common/error.cpp
    ui-components-qt.cpp
   )

add_library(${target_name} SHARED ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../external/
    ../
    ../app-kit/
    )

target_link_libraries(${target_name} katla-wm Qt5::Widgets)
