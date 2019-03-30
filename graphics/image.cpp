#include "image.h"

#include <cstring>

Image::Image (void) :
    m_pixels (nullptr),
    m_ref (nullptr),
    m_size {0,0},
    m_channels (0),
    m_step (0),
    m_depth (0)
{
}

Image::Image(const Size_32s& size, int channels, int depth) :
    m_size (size),
    m_channels (channels),
    m_depth (depth)
{
    m_step = size.width * channels * depth/8;

    m_pixels = new uint8_t [size_t(size.height) * m_step];

    m_ref = new std::atomic<int>();
    m_ref->operator++();
}

Image::Image(const Size_32s& size, int channels, int step, int depth) :
    m_size (size),
    m_channels (channels),
    m_step (step),
    m_depth (depth)
{
    m_pixels = new uint8_t [size_t(size.height) * step];

    m_ref = new std::atomic<int>();
    m_ref->operator++();
}

Image::Image(uint8_t* pixels, const Size_32s& size, int channels, int step, int depth) :
    m_pixels (pixels),
    m_ref (nullptr),
    m_size (size),
    m_channels (channels),
    m_step (step),
    m_depth (depth)
{
}

Image::Image (const Image& image) :
    m_size (image.size()),
    m_channels (image.channels()),
    m_step (image.step()),
    m_depth (image.depth())
{
    m_pixels = image.pixels();
    m_ref = image.m_ref;
    if (m_ref)
        m_ref->operator++();
}

Image::~Image (void)
{
    if (!m_ref)
        return;

    if (m_pixels && !m_ref->operator--())
    {
        delete[] m_pixels;
        m_pixels = 0;

        delete m_ref;
    }
}

void Image::clear (void)
{
    if (m_pixels && !m_ref->operator--())
    {
        delete[] m_pixels;
        delete m_ref;
    }

    m_pixels = 0;
    m_ref = 0;
}

Image& Image::operator= (const Image& image)
{
    if (&image != this)
    {
        if (m_pixels && !m_ref->operator--())
        {
            delete[] m_pixels;
            delete m_ref;
        }
    }

    m_size = image.size();
    m_channels = image.channels();
    m_step = image.step();
    m_depth = image.depth();

    m_pixels = image.m_pixels;
    m_ref = image.m_ref;

    if (m_ref)
        m_ref->operator++();

    return *this;
}

Image Image::copy (void)
{
    Image image (m_size, m_channels, m_step, m_depth);

    memcpy (image.pixels(), m_pixels, size_t(m_size.height) * m_step);
    return image;
}

bool Image::isNull (void) const
{
    return (m_pixels == NULL) || (m_size.width == 0) || (m_size.height == 0);
}

