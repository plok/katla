cmake_minimum_required(VERSION 3.10)
project(katla-gtk)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DKATLA_WINDOW_MANAGEMENT_CLIENT_GLFW_INDLL)

set(sources
    gtk-platform-builder.cpp
    gtk-application.cpp
    gtk-window.cpp
    gtk-window-factory.cpp
    gtk-button-impl.cpp
    gtk-column.cpp
    gtk-row.cpp
    gtk-text-impl.cpp
    gtk-list-view.cpp
    )

add_library(katla-gtk ${sources})

target_include_directories(katla-gtk PRIVATE
    .
    ..
    ../external/
    ../
    ../app-kit/
    ${GTK4_INCLUDE_DIRS}
    )

target_include_directories(katla-gtk PUBLIC
    ${GTK4_INCLUDE_DIRS})

target_link_directories(katla-gtk PUBLIC
    ${GTK4_LIBRARY_DIRS})

target_link_libraries(katla-gtk katla-wm katla-core ${GTK4_LIBRARIES})
