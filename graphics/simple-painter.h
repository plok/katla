#ifndef SIMPLE_PAINTER_H
#define SIMPLE_PAINTER_H

#include "painter.h"

#include "image.h"
#include "color.h"
#include "common/primitives.h"

class SimplePainter final : Painter
{
public:
    void drawRectangle (const Image& image, const Rect_32s& rect, const Color_8u_ARGB& color) override;    
};

#endif // SIMPLE_PAINTER_H
