#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "image.h"
#include "color.h"
#include "common/primitives.h"
#include "common/error.h"

class TextRenderer
{
public:
    TextRenderer() = default;
    virtual ~TextRenderer() = default;

    virtual ErrorPtr init() = 0;
    virtual void drawText(std::string text,
        std::string fontFile,
        Image& image,
        const Point_32s& offset,
        const Color_8u_ARGB& color) = 0;
    
};

#endif
