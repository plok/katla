cmake_minimum_required(VERSION 3.10)
project(katla-mqtt)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

find_package(Threads REQUIRED)
find_package(Mosquitto REQUIRED)

set(target_name katla-mqtt)

add_definitions(-DKATLA_MQTT_INDLL)

set(sources
    mqtt.cpp
    mqtt.h
    mqtt-client.cpp
    mqtt-client.h
    mqtt-errors.cpp
    mqtt-errors.h
    )

add_library(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../
    ../external/
    ${LIBMOSQUITTO_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} fmt katla-core ${MOSQUITTO_LIBRARIES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries (${target_name})

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
            target_link_libraries (${target_name} stdc++fs)
    endif()
endif()
