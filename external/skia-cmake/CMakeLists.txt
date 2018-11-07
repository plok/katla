cmake_minimum_required(VERSION 3.6)

project(skia)

find_package(PythonInterp 2.7 REQUIRED)
find_program(VIRTUALENV NAMES virtualenv virtualenv2)

add_custom_command(
    OUTPUT libskia
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build-skia.sh --binary-dir=${CMAKE_BINARY_DIR} --virtual-env=${VIRTUALENV} --python=${PYTHON_EXECUTABLE}
)

add_custom_target(skia ALL
    DEPENDS libskia
)

