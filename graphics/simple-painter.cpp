#include "simple-painter.h"

void SimplePainter::drawRectangle (
    const Image& image,
    const Rect_32s& rect,
    const Color_8u_ARGB& color)
{
    auto size = image.size();
    auto pixels = image.pixels();
    auto channels = image.channels();
    auto step = image.step();

    for (int y = rect.top; y <= rect.bottom; y++)
    {
        int y_offset = y * step;
        for (auto x = rect.left; x <= rect.right; x++)
        {
            int index = y_offset + x * channels;

            pixels[index+0] = color.a;
            pixels[index+1] = color.r;
            pixels[index+2] = color.g;
            pixels[index+3] = color.b;
        }
    }

}
