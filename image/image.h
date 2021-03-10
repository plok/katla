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


#ifndef IMAGE_H
#define IMAGE_H

#include "core/primitives.h"

#ifdef KATLA_USE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#endif

#include <atomic>

namespace katla {

class Image
{
public:
    enum class ImageType
    {
        TYPE_INVALID,
        TYPE_8U_C1 = 1,
        TYPE_8U_C3,
        TYPE_8U_C4,
        TYPE_16S_C1,
        TYPE_16S_C3,
        TYPE_16S_C4,
    };

    explicit Image (void);
    Image(const Size_32s& size, const ImageType& type);
    Image(const Size_32s& size, const ImageType& type, int step);
    Image(uint8_t* pixels, const Size_32s& size, const ImageType& type, int step);

    Image (const Image& image);
    Image (const Image& image, const Rect_32s& roi);

    virtual ~Image (void);

    Image& operator= (const Image& image);

    Image copy (void);

#ifdef KATLA_USE_OPENCV
    cv::Mat toMat(void) const;
    cv::Mat toNonOwningMat(void) const;
    static Image fromMat(cv::Mat src);
#endif

    void clear (void);
    bool isNull (void) const;

    uint8_t* buffer () const {return m_buffer;}
    uint8_t* pixels () const {return m_pixels;}
    Size_32s size () const {return m_size;}
    int channels () const;

    // TODO bitsPerPixel?
    int bytesPerPixel() const;

    int lineStride () const {return m_lineStride;}
    ImageType type() const { return m_type; }

    int totalSize() const;

    static ImageType typeFromArgs(int channels, int depth);

private:
    uint8_t* m_buffer {nullptr};
    std::atomic<int>* m_ref {nullptr};

    uint8_t* m_pixels {nullptr};

    Size_32s m_size {0,0};
    ImageType m_type {ImageType::TYPE_INVALID};

    int m_lineStride {0};
};

}

#endif // IMAGE_H
