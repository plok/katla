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


#ifndef KATLA_IMAGE_C_API_IMAGE_H
#define KATLA_IMAGE_C_API_IMAGE_H

#include "core/primitives.h"

#ifdef __cplusplus
    #include "katla/image/image.h"
    #include <atomic>

    typedef katla::Image* ImagePtr;
#else
    typedef void* ImagePtr;
#endif

#ifdef __cplusplus
using namespace katla;
extern "C" {
#endif

ImagePtr create_image(const struct Size_32s size, int channels, int depth);
ImagePtr create_image_with_step(const struct Size_32s size, int channels, int step, int depth);
ImagePtr create_image_from_existing_pixels(uint8_t* pixels, const struct Size_32s size, int channels, int step, int depth);

// TODO owning/non-owning variants
ImagePtr create_image_from_existing_roi(ImagePtr image, const struct Rect_32s roi);

void destroy_image(ImagePtr image);

ImagePtr read_image(char* imagePath);
void write_image_sync(char* imagePath, ImagePtr image);

struct Size_32s image_size(ImagePtr image);

int image_line_stride(ImagePtr image);
int image_channels(ImagePtr image);

void split(ImagePtr src, ImagePtr* dest);

#ifdef __cplusplus
}
#endif

#endif // IMAGE_H
