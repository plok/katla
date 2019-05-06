#ifndef FONT_FACE_CACHE_H
#define FONT_FACE_CACHE_H

#include "graphics-freetype/glyph.h"

#include <tuple>

class FontFaceCache
{
public:
    FontFaceCache();

    std::tuple<PixelGlyph, bool> find(uint32_t character);
    void insert(uint32_t character, PixelGlyph glyph);

private:
    std::map<uint32_t, PixelGlyph> _map;
};

#endif // FONT_FACE_CACHE_H
