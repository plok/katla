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

#include "image.h"

#include <assert.h>

#ifdef __cplusplus
    #include "katla/core/core.h"
    #include "katla/image/image.h"
    #include "katla/image/opencv-image-reader.h"
    #include "katla/image/opencv-image-writer.h"
    #include "katla/core/dispatcher-thread.h"
    #include <string>

    typedef katla::Image* ImagePtr;
#else
    typedef void* ImagePtr;
#endif

#ifdef __cplusplus
using namespace katla;
extern "C" {
#endif

ImagePtr create_image(const struct Size_32s size, int channels, int depth)
{
    Image::ImageType type = Image::typeFromArgs(channels, depth);
    return new Image(size, type);
}
ImagePtr create_image_with_step(const struct Size_32s size, int channels, int step, int depth)
{
    Image::ImageType type = Image::typeFromArgs(channels, depth);
    return new Image(size, type, step);
}
ImagePtr create_image_from_existing_pixels(uint8_t* pixels, const struct Size_32s size, int channels, int step, int depth)
{
    Image::ImageType type = Image::typeFromArgs(channels, depth);
    return new Image(pixels, size, type, step);
}

// TODO owning/non-owning variants
ImagePtr create_image_from_existing_roi(ImagePtr image, const struct Rect_32s roi)
{
    return new Image(*image, roi);
}

void destroy_image(ImagePtr image)
{
    assert(image != nullptr);

    delete image;
}

ImagePtr read_image(char* imagePath)
{
    auto path = std::string(imagePath);
    auto imagePtr = OpencvImageReader::read(path);
    if (!imagePtr) {
        return nullptr;
    }

    return new katla::Image(*imagePtr); // ok because of shallow copy
}

void write_image_sync(char* imagePath, ImagePtr image)
{
    auto path = std::string(imagePath);

    katla::Image imageCopy (*image);

    OpencvImageWriter::write(path, imageCopy);
}

Size_32s image_size(ImagePtr image) {
    return image->size();
}

int image_line_stride(Image* image) {
    return image->lineStride();
}

#ifdef __cplusplus
}
#endif
