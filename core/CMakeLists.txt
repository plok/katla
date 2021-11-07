cmake_minimum_required(VERSION 3.10)
project(katla-core)

include(${PROJECT_SOURCE_DIR}/../cmake/cmake-common.txt)

set(target_name katla-core)

add_definitions(-DKATLA_CORE_INDLL)

set(sources
        core.h

        error.cpp
        error.h

        core-errors.cpp
        core-errors.h
        
        stopwatch.cpp
        stopwatch.h

        string-utils.cpp
        string-utils.h

        thread.cpp
        thread.h
        worker-thread.cpp
        worker-thread.h

        observable.h
        observer.h
        subject.h
        )

IF (NOT WIN32)
        set(sources ${sources}
                posix-errors.cpp
                posix-errors.h

                posix-pipe.cpp
                posix-pipe.h

                posix-file.cpp
                posix-file.h

                posix-thread.cpp
        )

        IF(NOT APPLE)
                set(sources ${sources}
                        posix-socket.cpp
                        posix-socket.h
                )
        ENDIF()
ELSE()
        set(sources ${sources}
                win32-errors.cpp
                win32-errors.h

                win32-thread.cpp
                win32-thread.h
        )
ENDIF()

add_library(${target_name} ${sources})

target_include_directories(${target_name} PUBLIC
        ../
        ../../
        ../external/   # TODO make suitable for FindPackage
        ../external/gsl/include
        ${LIBGSL_INCLUDE_DIR}
        )

target_include_directories(${target_name} PRIVATE
        .
        )

target_link_libraries(${target_name} fmt Microsoft.GSL::GSL)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${target_name} Threads::Threads)

    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
        target_link_libraries(${target_name} stdc++fs)
    endif ()
endif ()
