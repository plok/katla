/***
 * Copyright 2019 The Katla Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "app-kit/core-application.h"

#include "compositor/wayland/client/wayland-display.h"

#include "graphics/image.h"
#include "graphics/simple-painter.h"
#include "graphics/image-processor.h"

#include "graphics-freetype/text-renderer-freetype.h"
#include "graphics-freetype/font-face-freetype.h"

#include "tiffio.h"

#include "string.h"
#include <iostream>
#include <vector>


void saveImage(const Image& image, std::string file)
{
    TIFF* tif = TIFFOpen(file.c_str(), "w");

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image.size().width);  // set the width of the image
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image.size().height);    // set the height of the image
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, image.channels());   // set number of channels per pixel
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, image.depth());    // set the size of the channels
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
    //   Some other essential fields to set that you do not have to understand for now.
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    uint16 extraSamples[1];
    extraSamples[0] = EXTRASAMPLE_ASSOCALPHA;

    TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, 1, &extraSamples );

    Image rgbaImage(image.size(), image.channels(), image.step(), image.depth());
    ImageProcessor::convertArgbToRgba(image, rgbaImage);

    tsize_t linebytes = rgbaImage.channels() * rgbaImage.size().width;     // length in memory of one row of pixel in the image.
    unsigned char *buf = NULL;        // buffer used to store the row of pixel information for writing to file
    //    Allocating memory to store the pixels of current row
    if (TIFFScanlineSize(tif) == linebytes)
        buf =(unsigned char *)_TIFFmalloc(linebytes);
    else
        buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));

    // We set the strip size of the file to be size of one row of pixels
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, rgbaImage.channels() * rgbaImage.size().width));

    //Now writing image to the file one strip at a time
    auto height = rgbaImage.size().height;
    auto pixels = rgbaImage.pixels();
    for (size_t row = 0; row < height; row++)
    {
        memcpy(buf, &pixels[(height-row-1)*linebytes], linebytes);    // check the index here, and figure out why not using h*linebytes
        if (TIFFWriteScanline(tif, buf, row, 0) < 0)
        break;
    }

    if (buf)
        _TIFFfree(buf);

    TIFFClose(tif);
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    std::cout << "Starting freetype-test with args: ";
    for(auto s : args)
        std::cout << s; std::cout << std::endl;

    CoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return EXIT_FAILURE;
    }
	
    Size_32s size { 640, 480 };
    Image image(size, 4, 640*4, 8);
    SimplePainter painter;
    painter.drawRectangle(image, Rect_32s {0, 0, 640, 480}, Color_8u_ARGB {255,255,255,255});
    painter.drawRectangle(image, Rect_32s {50, 150, 100, 200}, Color_8u_ARGB {255,255,0,0});
    painter.drawRectangle(image, Rect_32s {100, 200, 150, 250}, Color_8u_ARGB {255,0,255,0});
    painter.drawRectangle(image, Rect_32s {150, 250, 200, 300}, Color_8u_ARGB {255,0,0,255});

    std::cout << "Init freetype" << std::endl;

    TextRendererFreeType freetype;
    auto error = freetype.init();
    if (error) {
        std::cout << "Freetype error: " << error->name << " " << error->message << std::endl;
        return -1;
    }

    freetype.drawText("Hello World!", "/usr/share/fonts/TTF/Roboto-Regular.ttf", image, {150, 150}, Color_8u_ARGB {255,0,0,0});

    saveImage(image, "test.tif");

    return 0;
}
