#include "font-face-freetype.h"

#include <string>
#include <iostream>
#include <cassert>

FontFaceFreeType::FontFaceFreeType(FT_Library library) :
  _library(library)
{}

ErrorPtr FontFaceFreeType::load(std::string fileName)
{
    auto error = FT_New_Face(_library,
                     fileName.c_str(),                     
                     0,
                     &_face );
    if ( error ) {
        return Error::create("Loading failed!");
    }

    error = FT_Set_Char_Size(
          _face,    /* handle to face object           */
          0,       /* char_width in 1/64th of points  */
          16*64,   /* char_height in 1/64th of points */
          300,     /* horizontal device resolution    */
          300 );   /* vertical device resolution      */

    if ( error ) {
        return Error::create("Setting font size failed!");
    }

    // error = FT_Set_Pixel_Sizes(
    //       face,   /* handle to face object */
    //       0,      /* pixel_width           */
    //       16 );   /* pixel_height          */

    return Error::none();
}

BitmapGlyph FontFaceFreeType::drawGlyph(Point_32s offset, uint32_t character)
{
    auto index = FT_Get_Char_Index( _face, character );

    auto error = FT_Load_Glyph(
            _face,   /* handle to face object */
            index,   /* glyph index           */
            0 );  /* load flags, see below */

    if (_face->glyph->format != FT_GLYPH_FORMAT_BITMAP) 
    {
      error = FT_Render_Glyph( _face->glyph,   /* glyph slot  */
                            FT_RENDER_MODE_NORMAL ); /* render mode */
    }

    auto bitmap = _face->glyph->bitmap;
    auto bitmapPixels = bitmap.buffer;
    Size_32s bitmapSize = { static_cast<int32_t>(bitmap.width), static_cast<int32_t>(bitmap.rows) };

    Image image (bitmapSize, 1, 8);

    auto size = image.size();
    auto pixels = image.pixels();
    auto channels = image.channels();
    auto step = image.step();

    // TODO to flip or not to flip Y
    for (int y = 0; y < bitmapSize.height; y++)
    {
        int yOffset = y * bitmapSize.width;
        int yOffsetInv = ((bitmapSize.height - 1) - y) * bitmapSize.width;
        for (auto x = 0; x < bitmapSize.width; x++)
        {
            int index = yOffset + x;
            int indexInv = yOffsetInv + x;
            pixels[indexInv] = bitmapPixels[index];
        }
    }

    return {
        {
        /*character*/ character,
        /* left */ _face->glyph->bitmap_left * -1,
        /* top */ _face->glyph->bitmap_top * -1,
        /* advance */ { static_cast<int32_t>(_face->glyph->advance.x), static_cast<int32_t>(_face->glyph->advance.y) },
        /* adcender */ _face->ascender,
        /* descender */ _face->descender,
        /* height */ _face->height
        },
        image
    };

    // std::cout << "left" << _face->glyph->bitmap_left << std::endl;
    // std::cout << "top" << _face->glyph->bitmap_top << std::endl;

    //assert(bitmapSize.width + offset.x >= size.width);
    //assert(bitmapSize.height + offset.y >= size.width);

    // auto left = offset.x - _face->glyph->bitmap_left;
    // auto top = offset.y - _face->glyph->bitmap_top;

    
}


