#ifndef FONT_FACE_WRITER_H
#define FONT_FACE_WRITER_H

#include "graphics/image.h"
#include "graphics-freetype/font-face.h"
#include "graphics-freetype/font-face-cache.h"

#include "common/primitives.h"

#include <string>

class FontFaceWriter
{
public:
    FontFaceWriter(std::shared_ptr<FontFace> fontFace);
    virtual ~FontFaceWriter() = default;
    
    void drawText(std::string text,
        Image& image,
        const Point_32s& offset,
        const Color_8u_ARGB& color);
    
private:
    std::shared_ptr<FontFace> _fontFace;

    FontFaceCache _cache;
};

#endif // FONT_FACE_WRITER_H
