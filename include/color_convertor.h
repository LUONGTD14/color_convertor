#ifndef COLOR_CONVERTOR_H
#define COLOR_CONVERTOR_H

#include "constant.h"
#include "image.h"

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

class FileLoader;

class ColorConvertor
{
public:
    ColorConvertor() = default;
    ~ColorConvertor() = default;

    static YUVPixel rgb_to_yuv_R(const RGBPixel &rgb, const ColorStandard &standard, const ColorRange &range);
    static RGBPixel yuv_to_rgb_R(const YUVPixel &yuv, const ColorStandard &standard, const ColorRange &range);

    static YUV444pImage rgbImg_to_yuv444pImg_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range);
    static RGBImage yuv444pImg_to_rgbImg_R(const YUV444pImage &yuvImage, const ColorStandard &standard, const ColorRange &range);

    static YUV420pImage rgbImg_to_yuv420pImg_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range);
    static RGBImage yuv420pImg_to_rgbImg_R(const YUV420pImage &yuvImage, const ColorStandard &standard, const ColorRange &range);

    static NV12Image rgbImg_to_nv12Img_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range);
    static RGBImage nv12Img_to_rgbImg_R(const NV12Image &yuvImage, const ColorStandard &standard, const ColorRange &range);

    static NV21Image rgbImg_to_nv21Img_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range);
    static RGBImage nv21Img_to_rgbImg_R(const NV21Image &yuvImage, const ColorStandard &standard, const ColorRange &range);

    static YUV422pImage rgbImg_to_yuv422pImg_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range);
    static RGBImage yuv422pImg_to_rgbImg_R(const YUV422pImage &yuvImage, const ColorStandard &standard, const ColorRange &range);

    // =========== DOWNSAMPLE/UPSAMPLE ==============
    static YUV420pImage yuv444_to_yuv420p(const YUV444pImage &in);
    static YUV444pImage yuv420p_to_yuv444(const YUV420pImage &in);

    static NV12Image yuv444_to_nv12(const YUV444pImage &in);
    static YUV444pImage nv12_to_yuv444(const NV12Image &in);

    static NV21Image yuv444_to_nv21(const YUV444pImage &in);
    static YUV444pImage nv21_to_yuv444(const NV21Image &in);

    static YUV422pImage yuv444_to_yuv422p(const YUV444pImage &in);
    static YUV444pImage yuv422p_to_yuv444(const YUV422pImage &in);

    // ============ MAIN =========
    YUV444pImage convert_to_444(PixelFormat origin_fmt, const std::string &input_path, int width, int height, FileLoader &fl);
    void convert_from_444(const YUV444pImage &yuv444, PixelFormat next_fmt, const std::string &output_path, FileLoader &fl);
};

#endif