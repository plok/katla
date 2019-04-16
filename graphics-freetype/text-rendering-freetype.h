#ifndef TEXT_RENDERING_FREETYPE_H
#define TEXT_RENDERING_FREETYPE_H

#include "graphics/image.h"
#include "graphics/color.h"
#include "graphics/text-rendering.h"

#include "common/primitives.h"
#include "common/error.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderingFreeType : public TextRendering
{
public:
    virtual ErrorPtr init() override;

    virtual void drawText (const Image& image, const Point_32s& offset, const Color_8u_ARGB& color) override;
    
    FT_Library handle() {
        return _library;
    }
private:
    FT_Library  _library;
    
};

#endif // IMAGEPROCESSING_H
