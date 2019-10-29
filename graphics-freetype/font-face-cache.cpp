#include "font-face-cache.h"

FontFaceCache::FontFaceCache()
{}

std::optional<BitmapGlyph> FontFaceCache::find(uint32_t character)
{
    auto it = _map.find(character);
    if (it != _map.end()) {
        return it->second;
    }

    return std::nullopt;
}

void FontFaceCache::insert(uint32_t character, BitmapGlyph glyph)
{
    // TODO reserve characters for most common one in direct lookup
    _map[character] = glyph;
}



