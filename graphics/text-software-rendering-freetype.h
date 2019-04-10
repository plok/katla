#ifndef TEXT_SOFTWARE_RENDERING_FREETYPE_H
#define TEXT_SOFTWARE_RENDERING_FREETYPE_H

#include "text-rendering.h"

#include "image.h"
#include "color.h"
#include "common/primitives.h"

class TextSoftwareRenderingFreeType : public TextRendering
{
public:
    virtual void init() override;

    virtual void drawText (const Image& image, const Point_32s& offset, const Color_8u_ARGB& color) override;
    
private:
    
};

#endif // IMAGEPROCESSING_H
