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

#include "opencv-image-processor.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace katla {

OpencvImageProcessor::OpencvImageProcessor()
{
}

void OpencvImageProcessor::split(const Image& src, std::vector<Image>& dest) {
    auto srcMat = src.toNonOwningMat();

    std::vector<cv::Mat> destOpencv;
    for(auto& it : dest) {
        destOpencv.push_back(it.toNonOwningMat());
    }

    cv::split(srcMat, destOpencv);
}

}
