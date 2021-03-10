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


#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "image.h"

#include "lut.h"

#include "core/primitives.h"

namespace katla {

typedef Lut Histogram;

// TODO split
class ImageProcessor
{
public:
    ImageProcessor();
    virtual ~ImageProcessor() = default;

    static void createHistogram (const Image& image, Histogram& histogram);    
    static void normalizeHistogram (Histogram& histogram, const int max_value);

    static void generateLut (float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue, Lut& lut);
    static void applyLut (const Image* src, Image* dest, const Lut& lut);

    static Image fastScale (const Image& image, Size_32s minimumSize);
    static void scale (const Image* src, Image* dest, Size_32s size);

    static void applyGamma_16u (const Image* src, Image* dest);

    static void applyCameraMatrix_16u (const Image* src, Image* dest);

    static void fastDebayer_16u (const Image& src, Image& dest);

    static void NLdenoise_8u (const Image& src, Image& dest, int h, int hColor, int templateWindow, int searchWindow);

    static void medianFilter_8u (const Image& src, Image& dest, int kernelSize);
    static void medianFilter_16u (const Image& src, Image& dest, int kernelSize);

    static void applyProperties (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);

    static void convertArgbToRgba(const Image& src, Image& dest);

private:
    static void createHistogram_8u (const Image& image, Histogram& histogram);
    static void createHistogram_16u (const Image& image, Histogram& histogram);

    static void applyLut_8u (const Image* src, Image* dest, const Lut& lut);
    static void applyLut_16u8u (const Image* src, Image* dest, const Lut& lut);

    static Image fastScale_8u (const Image& image, int scale);
    static Image fastScale_16u (const Image& image, int scale);

    static void scale_8u (const Image* src, Image* dest, Size_32s size);

    static void applyProperties_8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);
    static void applyProperties_16u8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);

    static void convertRGBtoXYZ (float r, float g, float b, float& x, float& y, float& z);
    static void convertXYZtoRGB (float x, float y, float z, float& r, float& g, float& b);

    static void convertLABtoXYZ (float l, float a, float b, float& x, float& y, float& z);
    static void convertXYZtoLAB (float x, float y, float z, float& l, float& a, float& b);
};

}

#endif // IMAGE_PROCESSOR_H
