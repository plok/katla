#ifndef KATLA_APPKIT_H
#define KATLA_APPKIT_H

#ifdef _MSC_VER
    #ifdef KATLA_APPKIT_INDLL
    # define KATLA_APPKIT_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_APPKIT_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_APPKIT_DECLSPEC
#endif

#endif