#ifndef FONT_FACE_H
#define FONT_FACE_H

#include "graphics/image.h"
#include "graphics/color.h"
#include "graphics/text-rendering.h"

#include "graphics-freetype/bitmap-glyph.h"

#include "common/primitives.h"
#include "common/error.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FontFace
{
public:
    virtual ErrorPtr load(std::string fileName) = 0;
    
    virtual BitmapGlyph drawGlyph(Point_32s offset, uint32_t character) = 0;   
};

#endif // IMAGEPROCESSING_H
