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


#ifndef OPENCV_IMAGE_PROCESSOR_H
#define OPENCV_IMAGE_PROCESSOR_H

#include "image.h"
#include "image-processor.h"

#include "core/primitives.h"
#include <vector>

namespace katla {

class OpencvImageProcessor : public ImageProcessor
{
public:
    OpencvImageProcessor();
    virtual ~OpencvImageProcessor() = default;

    virtual void split(const Image& src, std::vector<Image>& dest);
};

}

#endif // IMAGE_PROCESSOR_H
