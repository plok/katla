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

#include "katla/core/core.h"
#include "katla/core/primitives.h"

#ifdef KATLA_USE_OPENCV
#include <opencv2/imgproc.hpp>
#endif

#include <cstring>

namespace katla {

Image::Image (void)
{
}

Image::Image(const Size_32s& size, const ImageType& type) :
    m_size (size),
    m_type(type)
{
    m_lineStride = size.width * channels() * bytesPerPixel();

    // TODO alignment
    m_buffer = new uint8_t [size_t(size.height) * m_lineStride];
    m_pixels = m_buffer;

    m_ref = new std::atomic<int>();
    m_ref->operator++();
}

Image::Image(const Size_32s& size, const ImageType& type, int lineStride) :
    m_size (size),
    m_type(type),
    m_lineStride (lineStride)
{
    m_buffer = new uint8_t [size_t(size.height) * m_lineStride];
    m_pixels = m_buffer;

    m_ref = new std::atomic<int>();
    m_ref->operator++();
}

Image::Image(uint8_t* pixels, const Size_32s& size, const ImageType& type, int lineStride) :
    m_buffer (nullptr),
    m_pixels (pixels),
    m_ref (nullptr),
    m_size (size),
    m_type (type),
    m_lineStride (lineStride)
{
}

Image::Image (const Image& image) :
    m_size (image.size()),
    m_type (image.type()),
    m_lineStride (image.lineStride())
{
    m_buffer = image.buffer();
    m_pixels = image.pixels();
    m_ref = image.m_ref;
    if (m_ref)
        m_ref->operator++();
}

Image::Image (const Image& image, const Rect_32s& roi) :
    m_size (Size_32s{roi.right-roi.left, roi.bottom-roi.top}),
    m_type (image.type()),
    m_lineStride (image.lineStride())
{
    m_buffer = image.buffer();
    m_pixels = image.pixels() + (roi.top * image.lineStride()) + (roi.left * image.bytesPerPixel());
    m_ref = image.m_ref;
    if (m_ref)
        m_ref->operator++();
}

Image::~Image (void)
{
    if (!m_ref)
        return;

    if (m_buffer && !m_ref->operator--())
    {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_pixels = nullptr;

        delete m_ref;
    }
}

void Image::clear (void)
{
    if (m_buffer && !m_ref->operator--())
    {
        delete[] m_buffer;
        delete m_ref;
    }

    m_buffer = nullptr;
    m_pixels = nullptr;
    m_ref = 0;
}

Image& Image::operator= (const Image& image)
{
    if (&image != this)
    {
        if (m_buffer && !m_ref->operator--())
        {
            delete[] m_buffer;
            delete m_ref;
        }
    }

    m_size = image.size();
    m_type = image.type();
    m_lineStride = image.lineStride();

    m_buffer = image.m_buffer;
    m_pixels = image.m_pixels;
    m_ref = image.m_ref;

    if (m_ref)
        m_ref->operator++();

    return *this;
}

Image Image::copy (void)
{
    // TODO is copying with lineStride the most efficient (check if lineStride > width*bytesPerPixel?
    Image image (m_size, m_type, m_lineStride);

    memcpy (image.buffer(), m_buffer, totalSize());
    return image;
}


int Image::channels() const
{
	int channels = 3;

	switch (m_type)
	{
	case Image::ImageType::TYPE_8U_C1: channels = 1; break;
	case Image::ImageType::TYPE_8U_C3: channels = 3; break;
	case Image::ImageType::TYPE_8U_C4: channels = 4; break;
	case Image::ImageType::TYPE_16S_C1: channels = 1; break;
	case Image::ImageType::TYPE_16S_C3: channels = 3; break;
	case Image::ImageType::TYPE_16S_C4: channels = 4; break;
    default: katla::fatal("unsupported image type!");
	}

	return channels;
}

int Image::bytesPerPixel() const
{
	int bytesPerPixel = 1;

	switch (m_type)
	{
	case ImageType::TYPE_8U_C1: bytesPerPixel = 1; break;
	case ImageType::TYPE_8U_C3: bytesPerPixel = 3; break;
	case ImageType::TYPE_8U_C4: bytesPerPixel = 4; break;
	case ImageType::TYPE_16S_C1: bytesPerPixel = 2; break;
	case ImageType::TYPE_16S_C3: bytesPerPixel = 6; break;
	case ImageType::TYPE_16S_C4: bytesPerPixel = 8; break;
    default: katla::fatal("unsupported image type!");
	}

	return bytesPerPixel;
}


#ifdef KATLA_USE_OPENCV
cv::Mat Image::toMat(void) const
{
    return toNonOwningMat().clone();
}

cv::Mat Image::toNonOwningMat(void) const
{
    int type = 0;
    switch(m_type) {
    case Image::ImageType::TYPE_8U_C1 : type = CV_8UC1; break;
    case Image::ImageType::TYPE_8U_C3 : type = CV_8UC3; break;
    case Image::ImageType::TYPE_8U_C4 : type = CV_8UC4; break;
    case Image::ImageType::TYPE_16S_C1 : type = CV_16SC1; break;
    case Image::ImageType::TYPE_16S_C3 : type = CV_16SC3; break;
    case Image::ImageType::TYPE_16S_C4 : type = CV_16SC4; break;
    default: katla::fatal("unsupported image type!");
    }

    cv::Mat m(m_size.height, m_size.width, type, pixels(), m_lineStride);

    return m;
}

Image Image::fromMat(cv::Mat src)
{
    Image::ImageType type = Image::ImageType::TYPE_8U_C3;

    switch(src.type()) {
    case CV_8UC1: type = Image::ImageType::TYPE_8U_C1; break;
    case CV_8UC3: type = Image::ImageType::TYPE_8U_C3; break;
    case CV_8UC4: type = Image::ImageType::TYPE_8U_C4; break;
    case CV_16SC1: type = Image::ImageType::TYPE_16S_C1; break;
    case CV_16SC3: type = Image::ImageType::TYPE_16S_C3; break;
    case CV_16SC4: type = Image::ImageType::TYPE_16S_C4; break;
    default: katla::fatal("unsupported image type!");
    }

    katla::Size_32s size  {src.size().width, src.size().height};

    Image result (size, type, src.step);

    assert ((src.total() * src.elemSize()) == result.totalSize());
    memcpy (result.pixels(), src.data, result.totalSize());

    return result;
}
#endif


bool Image::isNull (void) const
{
    return (m_pixels == NULL) || (m_size.width == 0) || (m_size.height == 0);
}

int Image::totalSize() const {
    // TODO not neccesary the buffer size
    return m_size.height * m_lineStride;
}

Image::ImageType Image::typeFromArgs(int channels, int depth) {
    Image::ImageType type = Image::ImageType::TYPE_INVALID;
    if (depth == 8) {
        if (channels == 1) {
            type = Image::ImageType::TYPE_8U_C1;
        }
        if (channels == 3) {
            type = Image::ImageType::TYPE_8U_C3;
        }
        if (channels == 4) {
            type = Image::ImageType::TYPE_8U_C4;
        }
    }
    if (depth == 16) {
        if (channels == 1) {
            type = Image::ImageType::TYPE_16S_C1;
        }
        if (channels == 3) {
            type = Image::ImageType::TYPE_16S_C3;
        }
        if (channels == 4) {
            type = Image::ImageType::TYPE_16S_C4;
        }
    }

    return type;
}

}
