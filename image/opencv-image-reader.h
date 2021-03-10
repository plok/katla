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


#ifndef KATLA_IMAGE_IMAGE_READER_H
#define KATLA_IMAGE_IMAGE_READER_H

#include "katla/image/image.h"

#include <string>
#include <memory>

namespace katla {

class OpencvImageReader
{
public:
    static std::unique_ptr<Image> read(std::string fileName);
};

}

#endif // KATLA_IMAGE_IMAGE_READER_H
