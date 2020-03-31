#ifndef KATLA_GRAPHICS_H
#define KATLA_GRAPHICS_H

#ifdef _MSC_VER
    #ifdef KATLA_GRAPHICS_INDLL
    # define KATLA_GRAPHICS_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_GRAPHICS_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_GRAPHICS_DECLSPEC
#endif

#endif