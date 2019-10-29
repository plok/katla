#ifndef FONT_FACE_FREETYPE_H
#define FONT_FACE_FREETYPE_H

#include "font-face.h"

#include "graphics/image.h"
#include "graphics/color.h"
#include "graphics/text-rendering.h"

#include "common/primitives.h"
#include "common/error.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FontFaceFreeType : public FontFace
{
public:
    FontFaceFreeType(FT_Library library);

    ErrorPtr load(std::string fileName);
    
    BitmapGlyph drawGlyph(Point_32s offset, uint32_t character);

private:
    FT_Library _library;
    FT_Face  _face;
    
};

#endif // IMAGEPROCESSING_H
