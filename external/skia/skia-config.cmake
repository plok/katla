find_package(PkgConfig)

if(NOT EXISTS "${skia_INCLUDE_DIR}")
  find_path(SKIA_INCLUDE_DIR
    NAMES core/SkSurface.h 
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../external/skia/skia/include

    DOC "Skia header files")
endif()

#if(NOT EXISTS "${SKIA_LIBRARY}")
#    find_library( SKIA_LIBRARY libskia
#        ${CMAKE_BINARY_DIR}/lib
#    )
#endif()

#if(SKIA_INCLUDE_DIR AND SKIA_LIBRARY)
if(SKIA_INCLUDE_DIR)
    set( SKIA_FOUND "YES" )
endif()

