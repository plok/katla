#ifndef IMAGE_DRAWING_H
#define IMAGE_DRAWING_H

#include "graphics/color.h"
#include "common/primitives.h"

class Image;

class ImageDrawing
{
public:
    virtual void drawRectangle (const Image& image, const Rect_32s& rect, const Color_8u_ARGB& color) = 0;
    
private:
    
};

#endif // IMAGEPROCESSING_H
