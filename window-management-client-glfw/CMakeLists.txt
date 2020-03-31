cmake_minimum_required(VERSION 3.10)
project(katla-window-management-client-glfw)

set(CMAKE_CXX_STANDARD 17)

find_library(GLFW_LIBRARIES NAMES glfw3 glfw)

add_definitions(-DKATLA_WINDOW_MANAGEMENT_CLIENT_GLFW_INDLL)

set(sources
    ../common/error.cpp
    
    glfw-window.cpp
    glfw-window-factory.cpp
    )

add_library(katla-window-management-client-glfw SHARED ${sources})

target_include_directories(katla-window-management-client-glfw PRIVATE
    .
    ..
    ../external/
    ../
    ../app-kit/
    )

target_link_libraries(katla-window-management-client-glfw katla-wm katla-graphics ${GLFW_LIBRARIES})
