#include "font-face-cache.h"

FontFaceCache::FontFaceCache() :
{}

std::tuple<PixelGlyph, bool> FontFaceCache::find(uint32_t character)
{
    auto it = _map.find(character);
    if (it == _map.end()) {
        return {PixelGlyph{}, false};
    }

    return {*it, true};
}

void FontFaceCache::insert(uint32_t character, PixelGlyph glyph)
{
    // TODO reserve characters for most common one in direct lookup
    _map[character] = glyph;
}



