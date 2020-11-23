cmake_minimum_required(VERSION 3.10)
project(katla)

include(cmake/cmake-common.txt)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(external/googletest)

SET(BUILD_SHARED_LIBS TRUE)
add_subdirectory(external/fmt)

option(LWS_WITH_STATIC "Build the static version of the library" OFF)
option(LWS_WITH_SSL "Include SSL support (defaults to OpenSSL or similar, mbedTLS if LWS_WITH_MBEDTLS is set)" OFF)
option(LWS_WITHOUT_TESTAPPS "Don't build the libwebsocket-test-apps" ON)
option(LWS_WITHOUT_TEST_SERVER "Don't build the test server" ON)
option(LWS_WITHOUT_TEST_SERVER_EXTPOLL "Don't build the test server version that uses external poll" ON)
option(LWS_WITHOUT_TEST_PING "Don't build the ping test application" ON)
option(LWS_WITHOUT_TEST_CLIENT "Don't build the client test application" ON)
option(LWS_WITH_SHARED "Build the shared version of the library" ON)
add_subdirectory(external/libwebsockets)

# using included libwebsockets as system version on ubuntu 1804 is too old
option(USE_SYSTEM_LIBWEBSOCKETS "use system libwebsockets" OFF)

add_subdirectory(core)
add_subdirectory(core/unit-tests)
add_subdirectory(core-uv)
add_subdirectory(websockets)
add_subdirectory(websockets/tests/integration)
add_subdirectory(sqlite)
add_subdirectory(sqlite/unit-tests)
add_subdirectory(mqtt)
add_subdirectory(mqtt/unit-tests)