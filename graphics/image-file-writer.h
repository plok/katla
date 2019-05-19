#ifndef IMAGE_FILE_WRITER_H
#define IMAGE_FILE_WRITER_H

#include "katla-graphics.h"

#include "graphics/image.h"
#include "common/size.h"

#include <atomic>
#include <string>

class KATLA_GRAPHICS_DECLSPEC ImageFileWriter
{
public:
    explicit ImageFileWriter (void);
    
    void write(const Image& image, std::string path);
};

#endif // IMAGE_H
