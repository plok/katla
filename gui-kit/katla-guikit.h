#ifndef KATLA_GUIKIT_H
#define KATLA_GUIKIT_H

#ifdef _MSC_VER
    #ifdef KATLA_GUIKIT_INDLL
    # define KATLA_GUIKIT_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_GUIKIT_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_GUIKIT_DECLSPEC
#endif

#endif