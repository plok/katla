#ifndef FONT_FACE_FREETYPE_H
#define FONT_FACE_FREETYPE_H

#include "graphics/image.h"
#include "graphics/color.h"
#include "graphics/text-rendering.h"

#include "common/primitives.h"
#include "common/error.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FontFaceFreeType
{
public:
    FontFaceFreeType(FT_Library library);

    ErrorPtr load(std::string fileName);
    
    void drawGlyph(Image& image, Point_32s offset, uint32_t character);

private:
    FT_Library _library;
    FT_Face  _face;
    
};

#endif // IMAGEPROCESSING_H
