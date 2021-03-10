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

#include "opencv-image-reader.h"

#include <string>
#include <memory>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace katla {

    std::unique_ptr<Image> OpencvImageReader::read(std::string fileName) {
        cv::Mat bgrImg = cv::imread(fileName);
        if (bgrImg.data != nullptr) {
            cv::Mat img;
            cv::cvtColor(bgrImg, img, cv::COLOR_BGR2RGBA);
            return std::make_unique<katla::Image>(katla::Image::fromMat(img));
        }

        return {};
    }
};
