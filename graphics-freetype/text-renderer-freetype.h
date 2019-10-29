#ifndef TEXT_RENDERER_FREETYPE_H
#define TEXT_RENDERER_FREETYPE_H

#include "font-face.h"
#include "font-face-writer.h"

#include "graphics/image.h"
#include "graphics/color.h"
#include "graphics/text-renderer.h"

#include "common/primitives.h"
#include "common/error.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>

class TextRendererFreeType final : public TextRenderer
{
public:
    TextRendererFreeType() = default;
    virtual ~TextRendererFreeType() = default;

    ErrorPtr init() override;

    std::unique_ptr<FontFace> loadFont(std::string file);
    
    void drawText(std::string text,
        std::string fontFile,
        Image& image,
        const Point_32s& offset,
        const Color_8u_ARGB& color) override;

    FT_Library handle() {
        return _library;
    }
private:
    std::unique_ptr<FontFaceWriter> createWriter(std::string file);

    FT_Library  _library;
    
};

#endif // TEXT_RENDERER_FREETYPE_H
