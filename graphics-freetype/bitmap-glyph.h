#ifndef BITMAP_GLYPH_H
#define BITMAP_GLYPH_H

#include "graphics-freetype/glyph-info.h"
#include "graphics/image.h"

#include "common/primitives.h"

struct BitmapGlyph
{
    GlyphInfo info;
    Image image;
};

#endif // PIXEL_GLYPH_H
