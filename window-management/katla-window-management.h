#ifndef KATLA_WINDOW_MANAGMENT_H
#define KATLA_WINDOW_MANAGMENT_H

#ifdef _MSC_VER
    #ifdef KATLA_WINDOW_MANAGEMENT_INDLL
    # define KATLA_WINDOW_MANAGEMENT_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_WINDOW_MANAGEMENT_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_WINDOW_MANAGEMENT_DECLSPEC
#endif

#endif