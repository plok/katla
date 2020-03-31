#ifndef KATLA_GPU_H
#define KATLA_GPU_H

#ifdef _MSC_VER
    #ifdef KATLA_GPU_INDLL
    # define KATLA_GPU_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_GPU_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_GPU_DECLSPEC
#endif

#endif
