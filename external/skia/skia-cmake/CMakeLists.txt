cmake_minimum_required(VERSION 3.6)

project(skia)

find_package(PythonInterp 2.7 REQUIRED)
find_program(VIRTUALENV NAMES virtualenv virtualenv2)

add_custom_command(
    OUTPUT build_skia
    OUTPUT ${CMAKE_BINARY_DIR}/lib/libskia.so
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build-skia.sh --src-dir=${CMAKE_CURRENT_SOURCE_DIR}/../ --binary-dir=${CMAKE_BINARY_DIR} --virtual-env=${VIRTUALENV} --python=${PYTHON_EXECUTABLE}
)

add_custom_target(skia_target ALL
    DEPENDS build_skia
)

add_library(skia SHARED IMPORTED GLOBAL skia_target)
add_dependencies(skia skia_target)

set_target_properties(skia
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libskia.so)

set (skia_include_directories_base ${CMAKE_CURRENT_SOURCE_DIR}/../skia/include)
set (skia_include_directories
    ${skia_include_directories_base}
    ${skia_include_directories_base}/core
    ${skia_include_directories_base}/config
    ${skia_include_directories_base}/gpu
    ${skia_include_directories_base}/utils
#    ${skia_include_directories_base}/private
)

target_include_directories(skia INTERFACE ${skia_include_directories})