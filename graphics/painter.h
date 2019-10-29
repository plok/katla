#ifndef PAINTER_H
#define PAINTER_H

#include "graphics/color.h"
#include "common/primitives.h"

class Image;

class Painter
{
public:
    virtual void drawRectangle (const Image& image, const Rect_32s& rect, const Color_8u_ARGB& color) = 0;    
};

#endif // PAINTER_H
