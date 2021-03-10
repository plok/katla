#ifndef POINT_H
#define POINT_H

#include <stdint.h>

struct Point_32s
{
    int32_t x;
    int32_t y;
};

struct Point_32f
{
    float x;
    float y;
};

typedef struct Point_32s Vector_32s;
typedef struct Point_32f Vector_32f;

#endif