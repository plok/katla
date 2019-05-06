#ifndef PIXEL_GLYPH_H
#define PIXEL_GLYPH_H

#include "graphics-freetype/glyph-info.h"
#include "graphics/image.h"

#include "common/primitives.h"

struct PixelGlyph
{
    GlyphInfo info;
    Image image;
};

#endif // PIXEL_GLYPH_H
