#ifndef FONT_FACE_CACHE_H
#define FONT_FACE_CACHE_H

#include "graphics-freetype/bitmap-glyph.h"

#include <tuple>
#include <optional>
#include <map>

class FontFaceCache
{
public:
    FontFaceCache();

    std::optional<BitmapGlyph> find(uint32_t character);
    void insert(uint32_t character, BitmapGlyph glyph);

private:
    std::map<uint32_t, BitmapGlyph> _map;
};

#endif // FONT_FACE_CACHE_H
