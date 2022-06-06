cmake_minimum_required(VERSION 3.10)
project(katla)

include(cmake/cmake-common.txt)

find_package(Threads REQUIRED)

option(USE_SYSTEM_GSL "Use system GSL library" OFF)
option(USE_SYSTEM_LIBWEBSOCKETS "use system libwebsockets" OFF) # using included libwebsockets as system version on ubuntu 1804 is too old
option(USE_SYSTEM_LIBUV "use system libuv" OFF)

option(BUILD_KATLA_UV "Build uv module" ON)
option(BUILD_KATLA_MQTT "Build mqtt module" ON)
option(BUILD_KATLA_SQLITE "Build sqlite module" ON)
option(BUILD_KATLA_WEBSOCKETS "Build websockets module" ON)

option(BUILD_KATLA_GTK3 "Build gtk-3 module" OFF)
option(BUILD_KATLA_GTK4 "Build gtk-4 module" OFF)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(external/googletest)

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
    ENDIF()
    add_subdirectory(core-uv)
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

add_subdirectory(ui-components)
add_subdirectory(window-management)

IF (BUILD_KATLA_GTK3)
    find_package(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK3 REQUIRED gtk3)

    add_subdirectory(gtk3)
    add_subdirectory(gtk3/integration-tests/gtk-application-test)
ENDIF()

IF (BUILD_KATLA_GTK4)
    find_package(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK4 REQUIRED gtk4)

    add_subdirectory(gtk4)
    add_subdirectory(gtk4/integration-tests/gtk-application-test)
ENDIF()