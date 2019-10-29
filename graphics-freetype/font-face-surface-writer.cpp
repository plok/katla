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
    for (int y = 0; y < srcSize.height; y++)
    {
        int yDestOffset = (offset.y + y) * destStep;
        int ySrcOffset = y * srcStep;

        for (auto x = 0; x < srcSize.width; x++)
        {
            int srcIndex = ySrcOffset + x;
            int destIndex = yDestOffset + (offset.x + x) * destChannels;

            if (srcPixels[srcIndex]) {
                float destAlpha = static_cast<float>(destPixels[destIndex+0]) / 255.f;

                // TODO support other depths
                float srcAlpha = (static_cast<float>(color.a) / 255) * (static_cast<float>(srcPixels[srcIndex]) / 255);
                float alpha = srcAlpha + (destAlpha * (1-srcAlpha));

                float destR = static_cast<float>(destPixels[destIndex+1]) / 255;
                float destG = static_cast<float>(destPixels[destIndex+2]) / 255;
                float destB = static_cast<float>(destPixels[destIndex+3]) / 255;

                float srcR = static_cast<float>(color.r) / 255;
                float srcG = static_cast<float>(color.g) / 255;
                float srcB = static_cast<float>(color.b) / 255;

                // TODO premultiply alpha?
                destPixels[destIndex+0] = static_cast<uint8_t>(alpha * 255);
                destPixels[destIndex+1] = static_cast<uint8_t>(((destR * destAlpha * (1.f-srcAlpha)) + (srcR * srcAlpha)) * (255.f / alpha));
                destPixels[destIndex+2] = static_cast<uint8_t>(((destG * destAlpha * (1.f-srcAlpha)) + (srcG * srcAlpha)) * (255.f / alpha));
                destPixels[destIndex+3] = static_cast<uint8_t>(((destB * destAlpha * (1.f-srcAlpha)) + (srcB * srcAlpha)) * (255.f / alpha));
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


