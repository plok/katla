#ifndef KATLA_IPCKIT_H
#define KATLA_IPCKIT_H

#ifdef _MSC_VER
    #ifdef KATLA_IPCKIT_INDLL
    # define KATLA_IPCKIT_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_IPCKIT_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_IPCKIT_DECLSPEC
#endif

#endif