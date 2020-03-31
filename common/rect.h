#ifndef RECT_H
#define RECT_H

#include <stdint.h>

struct Rect_32s
{
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct Rect_32f
{
    float left;
    float top;
    float right;
    float bottom;
};

#endif