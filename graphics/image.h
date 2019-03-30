#ifndef IMAGE_H
#define IMAGE_H

#include "katla-graphics.h"

#include "common/size.h"

#include <atomic>

class KATLA_GRAPHICS_DECLSPEC Image
{
public:
    explicit Image (void);
    Image(const Size_32s& size, int channels, int depth);
    Image(const Size_32s& size, int channels, int step, int depth);
    Image(uint8_t* pixels, const Size_32s& size, int channels, int step, int depth);

    Image (const Image& image);

    virtual ~Image (void);

    Image& operator= (const Image& image);

    Image copy (void);

    void clear (void);
    bool isNull (void) const;

    uint8_t* pixels () const {return m_pixels;}
    Size_32s size () const {return m_size;}
    int channels () const {return m_channels;}
    int step () const {return m_step;}
    int depth () const {return m_depth;}

private:
    uint8_t* m_pixels;
    std::atomic<int>* m_ref;

    Size_32s m_size;
    int m_channels;
    int m_step;
    int m_depth;
};

#endif // IMAGE_H
