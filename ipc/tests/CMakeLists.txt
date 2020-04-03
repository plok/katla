cmake_minimum_required(VERSION 3.10)
project(katla-ipc)

include(../../cmake/cmake-common.txt)

enable_testing()

find_package(UV)
find_package(GTest)
find_package(Fmt)

set(target_name katla-ipc-tests)

add_definitions(-DKATLA_IPCKIT_INDLL)

set(sources
    ../../common/error.cpp
    ipc-tests.cpp
    ipc-pipe-tests.cpp
    ipc-framing-tests.cpp
    )

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ../
    ../../
    ../../external/abseil-cpp/
    ../../external/outcome/
    ../../app-kit/
    ${LIBUV_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    )

target_link_libraries(${target_name} ${LIBUV_LIBRARIES} katla-ipc GTest::GTest GTest::Main fmt)

gtest_discover_tests(${target_name})
