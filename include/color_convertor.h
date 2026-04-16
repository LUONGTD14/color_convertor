#ifndef COLOR_CONVERTOR_H
#define COLOR_CONVERTOR_H

#include "constant.h"
#include <vector>

struct RGBPixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct YUVPixel
{
    uint8_t y;
    uint8_t u;
    uint8_t v;
};

struct RGBImage
{
    int width;
    int height;
    std::vector<uint8_t> data;
};

struct YUV420pImage
{
    int width;
    int height;
    std::vector<uint8_t> yPlane;
    std::vector<uint8_t> uPlane;
    std::vector<uint8_t> vPlane;
};

struct NV12Image
{
    int width;
    int height;
    std::vector<uint8_t> yPlane;
    std::vector<uint8_t> uvPlane; // U và V xen kẽ
};

struct NV21Image
{
    int width;
    int height;
    std::vector<uint8_t> yPlane;
    std::vector<uint8_t> vuPlane; // V và U xen kẽ
};

struct YUV422pImage
{
    int width;
    int height;
    std::vector<uint8_t> yPlane;
    std::vector<uint8_t> uPlane;
    std::vector<uint8_t> vPlane;
};

struct YUV444pImage
{
    int width;
    int height;
    std::vector<uint8_t> yPlane;
    std::vector<uint8_t> uPlane;
    std::vector<uint8_t> vPlane;
};

class ColorConvertor
{
public:
    ColorConvertor() = default;
    ~ColorConvertor() = default;

    YUVPixel rgb_to_yuv(const RGBPixel &rgb, const ColorStandard &standard);
    RGBPixel yuv_to_rgb(const YUVPixel &yuv, const ColorStandard &standard);

    YUV420pImage rgbImg_to_yuv420pImg(const RGBImage &rgbImage, const ColorStandard &standard);
    RGBImage yuv420pImg_to_rgbImg(const YUV420pImage &yuvImage, const ColorStandard &standard);

    NV12Image rgbImg_to_nv12Img(const RGBImage &rgbImage, const ColorStandard &standard);
    RGBImage nv12Img_to_rgbImg(const NV12Image &nv12Image, const ColorStandard &standard);

    NV21Image rgbImg_to_nv21Img(const RGBImage &rgbImage, const ColorStandard &standard);
    RGBImage nv21Img_to_rgbImg(const NV21Image &nv21Image, const ColorStandard &standard);

    YUV422pImage rgbImg_to_yuv422pImg(const RGBImage &rgbImage, const ColorStandard &standard);
    RGBImage yuv422pImg_to_rgbImg(const YUV422pImage &yuvImage, const ColorStandard &standard);

    YUV444pImage rgbImg_to_yuv444pImg(const RGBImage &rgbImage, const ColorStandard &standard);
    RGBImage yuv444pImg_to_rgbImg(const YUV444pImage &yuvImage, const ColorStandard &standard);
};

#endif