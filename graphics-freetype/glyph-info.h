#ifndef GLYPH_INFO_H
#define GLYPH_INFO_H

#include "common/primitives.h"

struct GlyphInfo
{
    uint32_t character;
    
    int left;
    int top;

    Vector_32s advance;

    int32_t ascender;    /* ascender in 26.6 frac. pixels          */
    int32_t descender;   /* descender in 26.6 frac. pixels         */
    int32_t height;      /* text height in 26.6 frac. pixels       */
};

#endif // GLYPH_INFO_H
