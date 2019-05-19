#include <iostream>
#include <fstream>
#include <string>

#include "image.h"

export module imagefilewriter;

//#include "image-file-writer.h"
w


export class KATLA_GRAPHICS_DECLSPEC ImageFileWriter
{
public:
    explicit ImageFileWriter (void);
    
    void write(const Image& image, std::string path);
};

ImageFileWriter::ImageFileWriter (void)
{}
    
void ImageFileWriter::write(const Image& image, std::string path)
{
    std::ofstream ostrm(path, std::ios::binary);
    //ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
    ostrm << 123 << "abc" << '\n';                      // text output
}