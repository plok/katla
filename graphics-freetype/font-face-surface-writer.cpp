#include "font-face-surface-writer.h"

#include <iostream>

FontFaceSurfaceWriter::FontFaceSurfaceWriter(std::shared_ptr<FontFace> fontFace) :
    _fontFace(fontFace)
{
    
}

void copyImage(Image& destImage, const Image& srcImage, const Point_32s& offset, const Color_8u_ARGB& color)
{
    auto destSize = destImage.size();
    auto destPixels = destImage.pixels();
    auto destChannels = destImage.channels();
    auto destStep = destImage.step();

    auto srcSize = srcImage.size();
    auto srcPixels = srcImage.pixels();
    auto srcChannels = srcImage.channels();
    auto srcStep = srcImage.step();

    // check image sizes
    if (offset.x + srcImage.size().width > destImage.size().width) {
        std::cout << "width > dest: " << offset.x << "-" << srcImage.size().width << "-" << destImage.size().width << std::endl;
        return;
    }
    if (offset.y + srcImage.size().height > destImage.size().height) {
        std::cout << "height > dest" << offset.y << "-" << srcImage.size().height << "-" << destImage.size().height << std::endl;
        return;
    }

    // TODO -> dup channels
    // test performance: blend ivp if
    for (int y = 0; y <= srcSize.height; y++)
    {
        int yDestOffset = (offset.y + y) * destStep;
        int ySrcOffset = y * srcStep;

        for (auto x = 0; x <= srcSize.width; x++)
        {
            int srcIndex = ySrcOffset + x;
            int destIndex = yDestOffset + (offset.x + x) * destChannels;

            if (srcPixels[srcIndex]) {
                destPixels[destIndex+0] = color.a;
                destPixels[destIndex+1] = color.r;
                destPixels[destIndex+2] = color.g;
                destPixels[destIndex+3] = color.b;
            }
        }
    }
}

void FontFaceSurfaceWriter::drawText (
    std::string text,
    Image& image,
    const Point_32s& offset,
    const Color_8u_ARGB& color)
{
    auto size = image.size();
    auto pixels = image.pixels();
    auto channels = image.channels();
    auto step = image.step();

    int x = 0;
    for(char& c : text) {
        if (auto glyphOpt = _cache.find(c); glyphOpt.has_value()) {
            auto& glyph = *glyphOpt;

            copyImage(image, glyph.image, {
                    offset.x + x,
                    offset.y
                },
                color);

                x += glyph.info.advance.x / 64;
        } else {
            auto glyph = _fontFace->drawGlyph({offset.x + x, offset.y}, c);

            copyImage(image, glyph.image, {
                    offset.x + x,
                    offset.y
                },
                color);

            _cache.insert(c, glyph);

            x += glyph.info.advance.x / 64;
        }

        std::cout << "x: " << x << std::endl;
    }
}


