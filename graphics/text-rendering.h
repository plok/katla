#ifndef TEXT_RENDERING_H
#define TEXT_RENDERING_H

#include "image.h"
#include "color.h"
#include "common/primitives.h"
#include "common/error.h"

class TextRendering
{
public:
    virtual ErrorPtr init() = 0;
    virtual void drawText(std::string text,
        std::string fontFile,
        Image& image,
        const Point_32s& offset,
        const Color_8u_ARGB& color) = 0;
    
private:
    
};

#endif
