#ifndef KATLA_WINDOW_MANAGMENT_WINCONTROL_H
#define KATLA_WINDOW_MANAGMENT_WINCONTROL_H

#ifdef _MSC_VER
    #ifdef KATLA_WINDOW_MANAGEMENT_WINCONTROL_INDLL
    # define KATLA_WINDOW_MANAGEMENT_WINCONTROL_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_WINDOW_MANAGEMENT_WINCONTROL_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_WINDOW_MANAGEMENT_WINCONTROL_DECLSPEC
#endif

#endif