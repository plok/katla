#include "text-rendering-freetype.h"

#include "font-face-freetype.h"
#include <iostream>

ErrorPtr TextRenderingFreeType::init()
{
    auto error = FT_Init_FreeType( &_library );
    if ( error ) {
        return Error::create("Init failed!");
    }

    return Error::none();
}

std::unique_ptr<FontFace> TextRenderingFreeType::loadFont(std::string file)
{
    auto face = new FontFaceFreeType(_library);
    auto error = face->load("/usr/share/fonts/TTF/Roboto-Regular.ttf");
    if (error) {
        std::cout << "Freetype load error: " << error->name << " " << error->message << std::endl;
        return {};
    }

    return std::unique_ptr<FontFace>(face);
}

std::unique_ptr<FontFaceSurfaceWriter> TextRenderingFreeType::createWriter(std::string file)
{
    auto fontFace = loadFont(file);
    auto writer = std::make_unique<FontFaceSurfaceWriter>(std::move(fontFace));
    return writer;
}

void TextRenderingFreeType::drawText (
    std::string text,
    std::string fontFile,
    Image& image,
    const Point_32s& offset,
    const Color_8u_ARGB& color)
{
    auto writer = createWriter(fontFile);

    writer->drawText(text, image, offset, color);
}

