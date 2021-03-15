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

#include "image-processor.h"

#include <cmath>
#include <exception>
#include <stdexcept>
#include <iostream>

#ifdef OPENCV
# include <opencv2/opencv.hpp>
# include <opencv2/imgproc/imgproc.hpp>
#endif

namespace katla {

ImageProcessor::ImageProcessor()
{
}

// void ImageProcessor::createHistogram (const Image& image, Histogram& histogram)
// {
//     if (image.bytesPerPixel() == 1)
//         createHistogram_8u (image, histogram);
//     if (image.bytesPerPixel() == 2)
//         createHistogram_16u (image, histogram);
// }

// void ImageProcessor::createHistogram_8u (const Image& image, Histogram& histogram)
// {
//     unsigned char* pixels = image.pixels();

//     Size_32s size = image.size();
//     unsigned int step = image.lineStride();
//     unsigned int channels = image.channels();

//     unsigned int r = 0;
//     unsigned int g = 0;
//     unsigned int b = 0;

//     int height = size.height;
//     int width = size.width;

//     int* red = histogram.red();
//     int* green = histogram.green();
//     int* blue = histogram.blue();

//     if (!red || !green || !blue)
//         return;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int index = (y*step) + (x*channels);

//             red[ pixels[index] ]++;
//             green[ pixels[index+1] ]++;
//             blue[ pixels[index+2] ]++;
//         }
//     }

//     normalizeHistogram (histogram, 1 << histogram.depth());
// }

// void ImageProcessor::createHistogram_16u (const Image& image, Histogram& histogram)
// {
//     unsigned char* pixels = image.pixels();

//     Size_32s size = image.size();
//     unsigned int step = image.lineStride();
//     unsigned int channels = image.channels();

//     unsigned int r = 0;
//     unsigned int g = 0;
//     unsigned int b = 0;

//     int height = size.height;
//     int width = size.width;

//     int* red = histogram.red();
//     int* green = histogram.green();
//     int* blue = histogram.blue();

//     if (!red || !green || !blue)
//         return;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (pixels + y*step) + x*channels;

//             red[ src_pixel[0] ]++;
//             green[ src_pixel[1] ]++;
//             blue[ src_pixel[2] ]++;
//         }
//     }

//     normalizeHistogram (histogram, 1 << histogram.depth());
// }

// void ImageProcessor::normalizeHistogram (Histogram& histogram, const int max_value)
// {
//     int* red = histogram.red();
//     int* green = histogram.green();
//     int* blue = histogram.blue();

//     if (!red || !green || !blue)
//         return;

//     float max = 0;
//     for (int i=1; i< max_value-1; i++)
//     {
//         if (red[i] > max)
//             max = red[i];
//         if (green[i] > max)
//             max = green[i];
//         if (blue[i] > max)
//             max = blue[i];
//     }

//     max = 255.f / max;

//     // TODO no inplace calc
//     if (max > 0)
//     {
//         for (int i=0; i<max_value; i++)
//         {
//             red[i] = float(red[i]) * max;
//             green[i] = float(green[i]) * max;
//             blue[i] = float(blue[i]) * max;
//         }
//     }
// }

// void ImageProcessor::generateLut (float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue, Lut& lut)
// {
//     int* red = lut.red();
//     int* green = lut.green();
//     int* blue = lut.blue();

//     if (!red || !green || !blue)
//         return;

//     int max_value = (1 << lut.depth());

//     brightness *= max_value;

//     for (int i=0; i<max_value; i++)
//     {
//         float gamma_value = float(max_value) * (pow ((float(i)/max_value), gamma));

//         float r = (contrast * gamma_value * wbRed) + brightness;
//         float g = (contrast * gamma_value * wbGreen) + brightness;
//         float b = (contrast * gamma_value * wbBlue) + brightness;

//         if (r < 0)
//             r = 0;
//         if (r > (max_value-1))
//             r = (max_value-1);
//         if (g < 0)
//             g = 0;
//         if (g > (max_value-1))
//             g = (max_value-1);
//         if (b < 0)
//             b = 0;
//         if (b > (max_value-1))
//             b = (max_value-1);

//         red[i] = r;
//         green[i] = g;
//         blue[i] = b;
//     }
// }

// void ImageProcessor::applyLut (const Image* src, Image* dest, const Lut& lut)
// {
//     if (src->bytesPerPixel() == 1 && dest->bytesPerPixel() == 1)
//         applyLut_8u( src, dest, lut);
//     if (src->bytesPerPixel() == 2 && dest->bytesPerPixel() == 1)
//         applyLut_16u8u( src, dest, lut);
// }

// void ImageProcessor::applyLut_8u (const Image* src, Image* dest, const Lut& lut)
// {
//     unsigned char* src_pixels = src->pixels();
//     unsigned char* dest_pixels = dest->pixels();

//     // TODO assume image size and format are the same
//     Size_32s size = src->size();
//     unsigned int step = src->lineStride();
//     unsigned int channels = src->channels();

//     int height = size.height;
//     int width = size.width;

//     int* red = lut.red();
//     int* green = lut.green();
//     int* blue = lut.blue();

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int index = (y*step) + (x*channels);

//             dest_pixels[index] =  red[ src_pixels[index] ];
//             dest_pixels[index+1] = green[ src_pixels[index+1] ];
//             dest_pixels[index+2] = blue[ src_pixels[index+2] ];
//         }
//     }
// }

// void ImageProcessor::applyLut_16u8u (const Image* src, Image* dest, const Lut& lut)
// {
//     unsigned char* src_pixels = src->pixels();
//     unsigned char* dest_pixels = dest->pixels();

//     // TODO assume image size and format are the same
//     Size_32s size = src->size();

//     unsigned int src_step = src->lineStride();
//     unsigned int dest_step = dest->lineStride();

//     unsigned int src_channels = src->channels();
//     unsigned int dest_channels = dest->channels();

//     int* red = lut.red();
//     int* green = lut.green();
//     int* blue = lut.blue();

//     int height = size.height;
//     int width = size.width;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int dest_index = (y*dest_step) + (x*dest_channels);

//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;

//             dest_pixels[dest_index] = red[ src_pixel[0] ] >> 8;
//             dest_pixels[dest_index+1] = green[ src_pixel[1] ] >> 8;
//             dest_pixels[dest_index+2] = blue[ src_pixel[2] ] >> 8;
//         }
//     }
// }

// void ImageProcessor::applyGamma_16u (const Image* src, Image* dest)
// {
//     unsigned char* src_pixels = src->pixels();
//     unsigned char* dest_pixels = dest->pixels();

//     // TODO assume image size and format are the same
//     Size_32s size = src->size();

//     unsigned int src_step = src->lineStride();
//     unsigned int dest_step = dest->lineStride();

//     unsigned int src_channels = src->channels();
//     unsigned int dest_channels = dest->channels();

//     int height = size.height;
//     int width = size.width;

//     float max_value_16b = (1 << 16);
//     float max_value_12b = (1 << 12);

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;
//             unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

//              float r = float(src_pixel[0]) / max_value_16b;
//              float g = float(src_pixel[1]) / max_value_16b;
//              float b = float(src_pixel[2]) / max_value_16b;

//              dest_pixel[0] = powf( r, 1.f / 2.2) * max_value_16b;
//              dest_pixel[1] = powf( g, 1.f / 2.2) * max_value_16b;
//              dest_pixel[2] = powf( b, 1.f / 2.2) * max_value_16b;
//         }
//     }
// }

// void ImageProcessor::applyCameraMatrix_16u (const Image* src, Image* dest)
// {
//     throw std::runtime_error("Not implemented");

// //    unsigned char* src_pixels = src->pixels();
// //    unsigned char* dest_pixels = dest->pixels();

// //    // TODO assume image size and format are the same
// //    QSize size = src->size();

// //    unsigned int src_step = src->step();
// //    unsigned int dest_step = dest->step();

// //    unsigned int src_channels = src->channels();
// //    unsigned int dest_channels = dest->channels();

// //    int height = size.height();
// //    int width = size.width();

// //    float max_value_16b = (1 << 16);
// //    float max_value_12b = (1 << 12);

// //    for(int y=0; y < height; y++)
// //    {
// //        for(int x=0; x < width; x++)
// //        {
// //            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;
// //            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

// //             float raw1 = float(src_pixel[0]) / max_value_16b;
// //             float raw2 = float(src_pixel[1]) / max_value_16b;
// //             float raw3 = float(src_pixel[2]) / max_value_16b;

// //             float r = raw1 * 1.82002354 + raw2 * -0.726660609 + raw3 * -0.0933628976;
// //             float g = raw1 * -0.167555362 + raw2 * 1.70425475 + raw3 * -0.536699355;
// //             float b = raw1 * 0.000880597509 + raw2 * -0.404351801 + raw3 * 1.40347123;

// ////             float x = raw1 * 1.82002354 + raw2 * -0.167555362 + raw3 * 0.000880597509;
// ////             float y = raw1 * -0.726660609 + raw2 * 1.70425475 + raw3 * -0.404351801;
// ////             float z = raw1 * -0.0933628976 + raw2 * -0.536699355 + raw3 * 1.40347123;

// ////             float r = raw1 * 5991.f / 10000 ;//+ raw2 * -4764.f/10000 + raw3 * -707.f/10000;
// ////             float g = raw1 * -1456.f / 10000 + raw2 * 12135.f/10000 + raw3 * 1425.f/10000;
// ////             float b = raw1 * -455.f/10000 + raw2 * -2980.f/10000 + raw3 * 6701.f/10000;

// ////             float x = raw1 * 5991.f / 10000 + raw2 * -1456.f / 10000 + raw3 * -455.f/10000;
// ////             float y = raw1 * -4764.f/10000 + raw2 * 12135.f/10000 + raw3 * -2980.f/10000;
// ////             float z = raw1 * -707.f/10000 + raw2 * 1425.f/10000 + raw3 * 6701.f/10000;

// ////             float r = x *  3.2406 + y * -1.5372 + z * -0.4986;
// ////             float g = x * -0.9689 + y *  1.8758 + z *  0.0415;
// ////             float b = x *  0.0557 + y * -0.2040 + z *  1.0570;

// ////             if ( r > 0.0031308 )
// ////                 r = 1.055 * ( pow (r, ( 1 / 2.4 )) ) - 0.055;
// ////             else
// ////                 r = 12.92 * r;

// ////             if ( g > 0.0031308 )
// ////                 g = 1.055 * ( pow (g, ( 1 / 2.4 )) ) - 0.055;
// ////             else
// ////                 g = 12.92 * g;

// ////             if ( b > 0.0031308 )
// ////                 b = 1.055 * ( pow (b, ( 1 / 2.4 )) ) - 0.055;
// ////             else
// ////                 b = 12.92 * b;


// //             if (r < 0)
// //                 r = 0;
// //             if (g < 0)
// //                 g = 0;
// //             if (b < 0)
// //                 b = 0;


// //             dest_pixel[0] = r * max_value_16b;
// //             dest_pixel[1] = g * max_value_16b;
// //             dest_pixel[2] = b * max_value_16b;
// //        }
// //    }
// }

// Image ImageProcessor::fastScale (const Image& image, Size_32s minimumSize)
// {
//     Size_32s image_size = image.size();

//     int scale = 0;
//     int i = 1;
//     while (((image_size.width >> i) > minimumSize.width ))
//     {
//         i++;
//         scale++;
//     }

//     Image scaled_image;
//     if (image.lineStride() == 8)
//         scaled_image = ImageProcessor::fastScale_8u(image, scale);
//     if (image.lineStride() == 16)
//         scaled_image = ImageProcessor::fastScale_16u(image, scale);

//     return scaled_image;
// }

// Image ImageProcessor::fastScale_8u (const Image& image, int scale)
// {
//     Size_32s scaled_size {
//         image.size().width >> scale,
//         image.size().height >> scale
//     };

//     Image dest_image (scaled_size, image.type());

//     unsigned char* src_pixels = image.pixels();
//     unsigned char* dest_pixels = dest_image.pixels();

//     // TODO assume image size and format are the same
//     Size_32s src_size = image.size();

//     unsigned int src_step = image.lineStride();
//     unsigned int dest_step = dest_image.lineStride();

//     unsigned int src_channels = image.channels();
//     unsigned int dest_channels = dest_image.channels();

//     int height = scaled_size.height;
//     int width = scaled_size.width;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int src_index = ((y << scale)*src_step) + ((x << scale)*src_channels);
//             unsigned int dest_index = (y*dest_step) + (x*dest_channels);

//             dest_pixels[dest_index] =  src_pixels[src_index];
//             dest_pixels[dest_index+1] = src_pixels[src_index+1];
//             dest_pixels[dest_index+2] = src_pixels[src_index+2];
//         }
//     }

//     return dest_image;
// }

// Image ImageProcessor::fastScale_16u (const Image& image, int scale)
// {
//     Size_32s scaled_size {
//         image.size().width >> scale,
//         image.size().height >> scale
//     };

//     Image dest_image (scaled_size, image.type());

//     unsigned char* src_pixels = image.pixels();
//     unsigned char* dest_pixels = dest_image.pixels();

//     // TODO assume image size and format are the same
//     Size_32s src_size = image.size();

//     unsigned int src_step = image.lineStride();
//     unsigned int dest_step = dest_image.lineStride();

//     unsigned int src_channels = image.channels();
//     unsigned int dest_channels = dest_image.channels();

//     int height = scaled_size.height;
//     int width = scaled_size.width;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + (y << scale)*src_step) + (x << scale)*src_channels;
//             unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

//             dest_pixel[0] = src_pixel[0];
//             dest_pixel[1] = src_pixel[1];
//             dest_pixel[2] = src_pixel[2];
//         }
//     }

//     return dest_image;
// }

// void ImageProcessor::scale(const Image *src, Image *dest, Size_32s size)
// {

//     if (src->bytesPerPixel() == 1)
//         ImageProcessor::scale_8u(src, dest, size);
//     //if (src->depth() == 16)
//     //    ImageProcessing::scale_16u(src, dest, size);
// }

// void ImageProcessor::scale_8u(const Image *src, Image *dest, Size_32s size)
// {
// #ifdef OPENCV
//     QSize qsize = src->size();
//     CvSize cv_size = {qsize.width(), qsize.height()};

//     cv::Mat src_mat (cv_size, CV_8UC3, reinterpret_cast <char*> (src->pixels()));
//     cv::Mat dest_mat (cv_size, CV_8UC3, reinterpret_cast <char*> (dest->pixels()));

// //    cv::medianBlur (src_mat, dest_mat, kernel);
// #endif
// }

// void ImageProcessor::fastDebayer_16u (const Image& src, Image& dest)
// {
//     // TODO for a fast half-size debayering, to a two pass method
//     // TODO use second green

//     // TODO or split a row in half size of cache

//     unsigned char* src_pixels = src.pixels();
//     unsigned char* dest_pixels = dest.pixels();

//     // TODO assume image size and format are the same
//     Size_32s src_size = src.size();

//     unsigned int src_step = src.lineStride();
//     unsigned int dest_step = dest.lineStride();

//     unsigned int src_channels = src.channels();
//     unsigned int dest_channels = dest.channels();

//     int height = src_size.height / 2;
//     int width = src_size.width / 2;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*2*src_step) + (x*2);
//             unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

//             dest_pixel[0] = src_pixel[1];
//             dest_pixel[1] = src_pixel[1];
//         }
//     }

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + ((y*2)+1)*src_step) + (x*2);
//             unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

//             dest_pixel[2] = 0;//src_pixel[0];
//         }
//     }
// }

// void ImageProcessor::NLdenoise_8u (const Image& src, Image& dest, int h, int hColor, int templateWindow, int searchWindow)
// {
//     try
//     {

// #ifdef OPENCV
//     QSize qsize = src.size();
//     CvSize size = {qsize.width(), qsize.height()};

//     qDebug() << src.channels();

//     int type = CV_8UC3;
//     if (src.channels() == 4)
//         type = CV_8UC4;

//     cv::Mat src_mat (size, type, reinterpret_cast <char*> (src.pixels()));
//     cv::Mat dest_mat (size, type, reinterpret_cast <char*> (dest.pixels()));

//     cv::fastNlMeansDenoisingColored(src_mat, dest_mat, h, hColor, templateWindow, searchWindow);
// #endif

//     } catch (...)
//     {}
// }

// void ImageProcessor::medianFilter_8u (const Image& src, Image& dest, int kernelSize)
// {
// #ifdef OPENCV
//     QSize qsize = src.size();
//     CvSize size = {qsize.width(), qsize.height()};

//     qDebug() << src.channels();

//     int type = CV_8UC3;
//     if (src.channels() == 4)
//         type = CV_8UC4;

//     cv::Mat src_mat (size, type, reinterpret_cast <char*> (src.pixels()));
//     cv::Mat dest_mat (size, type, reinterpret_cast <char*> (dest.pixels()));

//     cv::medianBlur (src_mat, dest_mat, kernelSize);
// #endif
// }

// void ImageProcessor::medianFilter_16u (const Image& src, Image& dest, int kernelSize)
// {
// #ifdef OPENCV
//     QSize qsize = src.size();
//     CvSize size = {qsize.width(), qsize.height()};

//     int type = CV_16UC3;
//     if (src.channels() == 4)
//         type = CV_16UC4;

//     cv::Mat src_mat (size, type, reinterpret_cast <char*> (src.pixels()));
//     cv::Mat dest_mat (size, type, reinterpret_cast <char*> (dest.pixels()));

//     cv::medianBlur (src_mat, dest_mat, kernelSize);
// #endif
// }

// void ImageProcessor::applyProperties (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue)
// {
//     if (src->bytesPerPixel() == 1 && dest->bytesPerPixel() == 1)
//         applyProperties_8u( src, dest, brightness, contrast, gamma, wbRed, wbGreen, wbBlue);
//     if (src->bytesPerPixel() == 2 && dest->bytesPerPixel() == 2)
//         applyProperties_16u8u( src, dest, brightness, contrast, gamma, wbRed, wbGreen, wbBlue);
// }

// void ImageProcessor::applyProperties_8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue )
// {
//     unsigned char* src_pixels = src->pixels();
//     unsigned char* dest_pixels = dest->pixels();

//     // TODO assume image size and format are the same
//     Size_32s size = src->size();

//     unsigned int src_step = src->lineStride();
//     unsigned int dest_step = dest->lineStride();

//     unsigned int channels = src->channels();

//     int height = size.height;
//     int width = size.width;

//     const float norm_value = (1 << 8);

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int index = (y*src_step) + (x*channels);

//             float r = static_cast<float> (src_pixels[index]) / norm_value;
//             float g = static_cast<float> (src_pixels[index+1]) / norm_value;
//             float b = static_cast<float> (src_pixels[index+2]) / norm_value;

//             float cx, y, z;
//             convertRGBtoXYZ(r, g, b, cx, y, z);

//             float l, a, lab_b;
//             convertXYZtoLAB(cx, y, z, l, a, lab_b);

//             l = l * contrast + (brightness * 100);

//             a = a * wbRed * wbGreen;
//             lab_b = lab_b * wbRed * wbBlue;

//             convertLABtoXYZ(l, a, lab_b, cx, y, z);

//             convertXYZtoRGB(cx, y, z, r, g, b);

//             r = r * 255.f;
//             g = g * 255.f;
//             b = b * 255.f;


//             if (r < 0)
//                 r = 0;
//             if (r > 255)
//                 r = 255;
//             if (g < 0)
//                 g = 0;
//             if (g > 255)
//                 g = 255;
//             if (b < 0)
//                 b = 0;
//             if (b > 255)
//                 b = 255;

//             dest_pixels[index] =  r;
//             dest_pixels[index+1] = g;
//             dest_pixels[index+2] = b;
//         }
//     }
// }

// void ImageProcessor::convertArgbToRgba(const Image& src, Image& dest)
// {
//     auto destSize = dest.size();
//     auto destPixels = dest.pixels();
//     auto destChannels = dest.channels();
//     auto destStep = dest.lineStride();

//     auto srcSize = src.size();
//     auto srcPixels = src.pixels();
//     auto srcChannels = src.channels();
//     auto srcStep = src.lineStride();

//     // check image sizes
//     if (src.size().width > dest.size().width) {
//         std::cout << "width > dest: " << src.size().width << "-" << dest.size().width << std::endl;
//         return;
//     }
//     if (src.size().height > dest.size().height) {
//         std::cout << "height > dest" << "-" << src.size().height << "-" << dest.size().height << std::endl;
//         return;
//     }

//     for (int y = 0; y <= srcSize.height; y++)
//     {
//         int yDestOffset = y * destStep;
//         int ySrcOffset = y * srcStep;

//         for (auto x = 0; x <= srcSize.width; x++)
//         {
//             int srcIndex = ySrcOffset + x * srcChannels;
//             int destIndex = yDestOffset + x * destChannels;

//             destPixels[destIndex+0] = srcPixels[srcIndex + 1];
//             destPixels[destIndex+1] = srcPixels[srcIndex + 2];
//             destPixels[destIndex+2] = srcPixels[srcIndex + 3];
//             destPixels[destIndex+3] = srcPixels[srcIndex + 0];
//         }
//     }
// }

// void ImageProcessor::applyProperties_16u8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue )
// {
//     unsigned char* src_pixels = src->pixels();
//     unsigned char* dest_pixels = dest->pixels();

//     // TODO assume image size and format are the same
//     Size_32s size = src->size();

//     unsigned int src_step = src->lineStride();
//     unsigned int dest_step = dest->lineStride();

//     unsigned int channels = src->channels();

//     const float norm_value = (1 << 16);

//     int height = size.height;
//     int width = size.width;

//     for(int y=0; y < height; y++)
//     {
//         for(int x=0; x < width; x++)
//         {
//             unsigned int dest_index = (y*dest_step) + (x*channels);

//             unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*channels;

//             float r = static_cast<float> (src_pixel[0]) / norm_value;
//             float g = static_cast<float> (src_pixel[1]) / norm_value;
//             float b = static_cast<float> (src_pixel[2]) / norm_value;

//             float cx, y, z;
//             convertRGBtoXYZ(r, g, b, cx, y, z);

//             float l, a, lab_b;
//             convertXYZtoLAB(cx, y, z, l, a, lab_b);

//             l = l * contrast + (brightness * 100);

//             a = a * wbRed * wbGreen;
//             lab_b = lab_b * wbRed * wbBlue;

//             convertLABtoXYZ(l, a, lab_b, cx, y, z);

//             convertXYZtoRGB(cx, y, z, r, g, b);

//             r = r * 255.f;
//             g = g * 255.f;
//             b = b * 255.f;


//             if (r < 0)
//                 r = 0;
//             if (r > 255)
//                 r = 255;
//             if (g < 0)
//                 g = 0;
//             if (g > 255)
//                 g = 255;
//             if (b < 0)
//                 b = 0;
//             if (b > 255)
//                 b = 255;

//             dest_pixels[dest_index] =  r;
//             dest_pixels[dest_index+1] = g;
//             dest_pixels[dest_index+2] = b;
//         }
//     }
// }

// void ImageProcessor::convertRGBtoXYZ (float r, float g, float b, float& x, float& y, float& z)
// {
//     if ( r > 0.04045 )
//         r = powf( (( r + 0.055 ) / 1.055 ), 2.4);
//     else
//         r = r / 12.92;

//     if ( g > 0.04045 )
//         g = powf( (( g + 0.055 ) / 1.055 ), 2.4);
//     else
//         g = g / 12.92;

//     if ( b > 0.04045 )
//         b = powf( (( b + 0.055 ) / 1.055 ), 2.4);
//     else
//         b = b / 12.92;

//     r = r * 100.f;
//     g = g * 100.f;
//     b = b * 100.f;

//     //Observer. = 2°, Illuminant = D65
//     x = r * 0.4124 + g * 0.3576 + b * 0.1805;
//     y = r * 0.2126 + g * 0.7152 + b * 0.0722;
//     z = r * 0.0193 + g * 0.1192 + b * 0.9505;
// }

// void ImageProcessor::convertXYZtoRGB (float x, float y, float z, float& r, float& g, float& b)
// {
//     x = x / 100.f;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
//     y = y / 100.f;        //Y from 0 to 100.000
//     z = z / 100.f;        //Z from 0 to 108.883

//     r = x *  3.2406 + y * -1.5372 + z * -0.4986;
//     g = x * -0.9689 + y *  1.8758 + z *  0.0415;
//     b = x *  0.0557 + y * -0.2040 + z *  1.0570;

//     if ( r > 0.0031308 )
//         r = 1.055 * ( powf (r, ( 1 / 2.4 )) ) - 0.055;
//     else
//         r = 12.92 * r;

//     if ( g > 0.0031308 )
//         g = 1.055 * ( powf (g, ( 1 / 2.4 )) ) - 0.055;
//     else
//         g = 12.92 * g;

// //    if ( b > 0.0031308 )
// //        b = 1.055 * ( powf (b, ( ImageProcessing::1 / 2.4 )) ) - 0.055;
// //    else
// //        b = 12.92 * b;
// }

// void ImageProcessor::convertLABtoXYZ (float l, float a, float b, float& x, float& y, float& z)
// {
//     y = ( l + 16.f ) / 116.f;
//     x = (a / 500.f) + y;
//     z = y - (b / 200.f);

//     if ( powf (y,3) > 0.008856 )
//         y= powf(y,3);
//     else
//         y = ( y - (16.f / 116.f) ) / 7.787;

//     if ( powf (x,3) > 0.008856 )
//         x = powf (x, 3);
//     else
//         x = ( x - (16.f / 116.f) ) / 7.787;

//     if ( powf (z,3) > 0.008856 )
//         z = powf(z, 3);
//     else
//         z = ( z - (16.f / 116.f) ) / 7.787;

//     x = 95.047 * x;     //ref_X =       Observer= 2°, Illuminant= D65
//     y = 100.000 * y;     //ref_Y =
//     z = 108.883 * z;     //ref_Z =
// }

// void ImageProcessor::convertXYZtoLAB (float x, float y, float z, float& l, float& a, float& b)
// {
//     x = x / 95.047;          //ref_X =     Observer= 2°, Illuminant= D65
//     y = y / 100.000;          //ref_Y =
//     z = z / 108.883;          //ref_Z =

//     if ( x > 0.008856 )
//         x = powf (x, ( 1.f/3.f ));
//     else
//         x = ( 7.787 * x ) + ( 16.f / 116.f );

//     if ( y > 0.008856 )
//         y = powf (y, ( 1.f/3.f ));
//     else
//         y = ( 7.787 * y ) + ( 16.f / 116.f );

//     if ( z > 0.008856 )
//         z = powf (z, ( 1.f/3.f ));
//     else
//         z = ( 7.787 * z ) + ( 16.f / 116.f );

//     l = ( 116.f * y ) - 16;
//     a = 500.f * ( x - y );
//     b = 200.f * ( y - z );
// }

}
