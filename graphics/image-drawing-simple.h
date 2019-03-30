#ifndef IMAGE_DRAWING_SIMPLE_H
#define IMAGE_DRAWING_SIMPLE_H

#include "image-drawing.h"

#include "image.h"
#include "color.h"
#include "common/primitives.h"

class ImageDrawingSimple : public ImageDrawing
{
public:
    virtual void drawRectangle (const Image& image, const Rect_32s& rect, const Color_8u_ARGB& color) override;
    
private:
    
};

#endif // IMAGEPROCESSING_H
