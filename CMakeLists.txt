cmake_minimum_required(VERSION 3.10)
project(katla)

include(cmake/cmake-common.txt)
IF (PROJECT_IS_TOP_LEVEL)
    include(cmake/cmake-warnings.txt)
ENDIF()

find_package(Threads REQUIRED)

option(USE_SYSTEM_GSL "Use system GSL library" OFF)
option(USE_SYSTEM_LIBWEBSOCKETS "use system libwebsockets" OFF) # using included libwebsockets as system version on ubuntu 1804 is too old
option(USE_SYSTEM_LIBUV "use system libuv" OFF)
option(USE_SYSTEM_OPENCV "use system opencv" ON)

set(BUILD_KATLA_UV_DEFAULT ON)
set(BUILD_KATLA_SQLITE_DEFAULT ON)
set(BUILD_KATLA_IMAGE_DEFAULT ON)
set(BUILD_KATLA_MQTT_DEFAULT ON)
set(BUILD_KATLA_DART_DEFAULT ON)
set(BUILD_KATLA_WEBSOCKETS_DEFAULT OFF) # still experimental

IF (WIN32)
    set(BUILD_KATLA_UV_DEFAULT OFF) # TODO build error
    set(BUILD_KATLA_SQLITE_DEFAULT OFF) # TODO sqlite currenly only on installed in linux ci
    set(BUILD_KATLA_IMAGE_DEFAULT OFF) # TODO opencv currenly only on installed in linux ci
    set(BUILD_KATLA_MQTT_DEFAULT OFF) # TODO mosquitto currenly only on installed in linux ci
    set(BUILD_KATLA_DART_DEFAULT OFF) # TODO depends on katla-image
ENDIF()

IF (APPLE)
    set(BUILD_KATLA_SQLITE_DEFAULT OFF) # TODO sqlite currenly only on installed in linux ci
    set(BUILD_KATLA_IMAGE_DEFAULT OFF) # TODO opencv currenly only on installed in linux ci
    set(BUILD_KATLA_MQTT_DEFAULT OFF) # TODO mosquitto currenly only on installed in linux ci
    set(BUILD_KATLA_DART_DEFAULT OFF) # TODO depends on katla-image
ENDIF()

option(BUILD_KATLA_UV "Build uv module" BUILD_KATLA_UV_DEFAULT)
option(BUILD_KATLA_SQLITE "Build sqlite module" BUILD_KATLA_SQLITE_DEFAULT)
option(BUILD_KATLA_IMAGE "Build image module" BUILD_KATLA_IMAGE_DEFAULT)
option(BUILD_KATLA_MQTT "Build mqtt module" BUILD_KATLA_MQTT_DEFAULT)
option(BUILD_KATLA_DART "Build dart module" BUILD_KATLA_DART_DEFAULT)
option(BUILD_KATLA_WEBSOCKETS "Build websockets module" BUILD_KATLA_WEBSOCKETS_DEFAULT)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(external/googletest)
target_compile_options(gtest PRIVATE -Wno-error=maybe-uninitialized -Wno-maybe-uninitialized)


add_subdirectory(external/fmt)

IF(USE_SYSTEM_GSL)
    find_package(GSL REQUIRED)
    add_library(Microsoft.GSL::GSL ALIAS GSL::gsl)
ELSE()
    add_subdirectory(external/gsl)
    include_directories(external/gsl/include)
ENDIF()

add_subdirectory(core)
IF (NOT WIN32 AND NOT APPLE) # currently only posix tests are defined
    add_subdirectory(core/unit-tests)
ENDIF()

IF(BUILD_KATLA_UV)
    IF(USE_SYSTEM_LIBWEBSOCKETS)
        find_package(Uv REQUIRED)
    ELSE()
        add_subdirectory(external/libuv)
        target_compile_options(uv PRIVATE -Wno-sign-compare -Wno-error=sign-compare)
        target_compile_options(uv_a PRIVATE -Wno-sign-compare -Wno-error=sign-compare)
    ENDIF()
    add_subdirectory(core-uv)
    add_subdirectory(core-uv/unit-tests)
ENDIF()

IF(BUILD_KATLA_WEBSOCKETS)
    IF(USE_SYSTEM_LIBWEBSOCKETS)
        find_package(Websockets REQUIRED)
    ELSE()
        option(LWS_WITH_STATIC "Build the static version of the library" OFF)
        option(LWS_WITH_SSL "Include SSL support (defaults to OpenSSL or similar, mbedTLS if LWS_WITH_MBEDTLS is set)" OFF)
        option(LWS_WITHOUT_TESTAPPS "Don't build the libwebsocket-test-apps" ON)
        option(LWS_WITHOUT_TEST_SERVER "Don't build the test server" ON)
        option(LWS_WITHOUT_TEST_SERVER_EXTPOLL "Don't build the test server version that uses external poll" ON)
        option(LWS_WITHOUT_TEST_PING "Don't build the ping test application" ON)
        option(LWS_WITHOUT_TEST_CLIENT "Don't build the client test application" ON)
        option(LWS_WITH_SHARED "Build the shared version of the library" ON)

        SET(POP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
        SET(BUILD_SHARED_LIBS TRUE)
        add_subdirectory(external/libwebsockets)
        target_compile_options(websockets_shared PRIVATE -Wno-error=shadow)
        target_include_directories(websockets_shared INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/libwebsockets/include/>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/libwebsockets/include/libwebsockets>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/external/libwebsockets>
        )
        SET(BUILD_SHARED_LIBS ${POP_BUILD_SHARED_LIBS})
    ENDIF()

    add_subdirectory(websockets)
    add_subdirectory(websockets/tests/integration)
ENDIF()

IF(BUILD_KATLA_SQLITE)
    find_package(SQLite3 REQUIRED)

    add_subdirectory(sqlite)
    add_subdirectory(sqlite/unit-tests)
ENDIF()

IF(BUILD_KATLA_MQTT)
    add_subdirectory(mqtt)
    add_subdirectory(mqtt/unit-tests)
ENDIF()

IF (BUILD_KATLA_IMAGE)
    if(${USE_SYSTEM_OPENCV})
        find_package(OpenCV REQUIRED)
    endif(${USE_SYSTEM_OPENCV})
    add_definitions(-DKATLA_USE_OPENCV)

    add_subdirectory(image)
    add_subdirectory(image/unit-tests)
ENDIF()

IF (BUILD_KATLA_DART)
    add_definitions(-DKATLA_USE_DART)
    add_subdirectory(dart)
ENDIF()
