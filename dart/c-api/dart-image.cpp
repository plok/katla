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

#include "dart-image.h"

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

#include "dart-api/dart_api_dl.h"

#ifdef __cplusplus
using namespace katla;
extern "C" {
#endif

int dart_init_ffi(void* initializeApiDLData)
{
    auto intPtr = Dart_InitializeApiDL(initializeApiDLData);
    return intPtr;
}

void dart_write_image(char* imagePath, ImagePtr image, int64_t nativePort)
{
    auto path = std::string(imagePath);
 
    auto dispatcherResult = katla::DispatcherThread::getDefault();
    if (!dispatcherResult) {
        katla::printError("failed getting default dispatcher thread!");
        return;
    }

    katla::Image imageCopy (*image);

    dispatcherResult.value().get().dispatch([path, imageCopy, nativePort]() {
        OpencvImageWriter::write(path, imageCopy);

        Dart_PostInteger_DL(nativePort, 1);
    });
}

int dart_image_width(ImagePtr image) {
    return image->size().width;
}
int dart_image_height(ImagePtr image) {
    return image->size().height;
}

int image_line_stride(Image* image) {
    return image->lineStride();
}

#ifdef __cplusplus
}
#endif
