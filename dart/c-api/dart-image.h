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


#ifndef DART_C_API_DART_IMAGE_H
#define DART_C_API_DART_IMAGE_H

#include "core/primitives.h"

#ifdef __cplusplus
    #include "katla/image/image.h"
    #include <atomic>

    #include "dart-api/dart_api.h"
    #include "dart-api/dart_native_api.h"

    typedef katla::Image* ImagePtr;
#else
    typedef void* ImagePtr;
#endif

#ifdef __cplusplus
using namespace katla;
extern "C" {
#endif

int dart_init_ffi(void* initializeApiDLData);

void dart_write_image(char* imagePath, ImagePtr image, int64_t nativePort);

// because dart ffigen doesnt support returning structs yet
int dart_image_width(ImagePtr image);
int dart_image_height(ImagePtr image);

#ifdef __cplusplus
}
#endif

#endif // DART_C_API_DART_IMAGE_H
