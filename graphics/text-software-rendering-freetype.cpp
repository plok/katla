#include "text-software-rendering-freetype.h"

#include <ft2build.h>
#include FT_FREETYPE_H


void TextSoftwareRenderingFreeType::init()
{
    
}

void TextSoftwareRenderingFreeType::drawText (
    const Image& image,
    const Point_32s& offset,
    const Color_8u_ARGB& color)
{
    auto size = image.size();
    auto pixels = image.pixels();
    auto channels = image.channels();
    auto step = image.step();

}
