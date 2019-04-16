#include "text-rendering-freetype.h"

ErrorPtr TextRenderingFreeType::init()
{
    auto error = FT_Init_FreeType( &_library );
    if ( error ) {
        return Error::create("Init failed!");
    }

    return Error::none();
}

void TextRenderingFreeType::drawText (
    const Image& image,
    const Point_32s& offset,
    const Color_8u_ARGB& color)
{
    auto size = image.size();
    auto pixels = image.pixels();
    auto channels = image.channels();
    auto step = image.step();


}
