#include "../include/color_convertor.h"
#include "../include/utils.h"
#include "../include/file_loader.h"
#include <iostream>

int main(int argc, char *argv[])
{

    if (argc != 9)
    {
        std::cout << "Usage:\n";
        std::cout << "color_convertor input_path output_path width height "
                     "origin_format next_format color_standard color_range\n";
        return -1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    int width = std::stoi(argv[3]);
    int height = std::stoi(argv[4]);
    std::string origin_fmt_str = argv[5];
    std::string next_fmt_str = argv[6];
    std::string color_std_str = argv[7];
    std::string color_range_str = argv[8];

    ColorConvertor cc;
    FileLoader fl;

    PixelFormat origin_fmt = Utils::getPixelFormatFromString(origin_fmt_str);
    PixelFormat next_fmt = Utils::getPixelFormatFromString(next_fmt_str);
    ColorStandard color_std = Utils::getColorStandardFromString(color_std_str);
    ColorRange color_range = Utils::getColorRangeFromString(color_range_str);

    // ================= CONVERT =================
    if (origin_fmt == PixelFormat::RGB24 && next_fmt == PixelFormat::YUV420P)
    {
        RGBImage rgb;
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
        YUV420pImage yuv = cc.rgbImg_to_yuv420pImg_R(rgb, color_std, color_range);
        if (!fl.saveYUV420pImage(output_path, yuv))
        {
            std::cerr << "Save YUV420 failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::RGB24 && next_fmt == PixelFormat::NV12)
    {
        RGBImage rgb;
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
        NV12Image yuv = cc.rgbImg_to_nv12Img_R(rgb, color_std, color_range);
        if (!fl.saveNV12Image(output_path, yuv))
        {
            std::cerr << "Save NV12 failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::RGB24 && next_fmt == PixelFormat::NV21)
    {
        RGBImage rgb;
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
        NV21Image yuv = cc.rgbImg_to_nv21Img_R(rgb, color_std, color_range);
        if (!fl.saveNV21Image(output_path, yuv))
        {
            std::cerr << "Save NV21 failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::RGB24 && next_fmt == PixelFormat::YUV422P)
    {
        RGBImage rgb;
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
        YUV422pImage yuv = cc.rgbImg_to_yuv422pImg_R(rgb, color_std, color_range);
        if (!fl.saveYUV422pImage(output_path, yuv))
        {
            std::cerr << "Save YUV422 failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::RGB24 && next_fmt == PixelFormat::YUV444P)
    {
        RGBImage rgb;
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
        YUV444pImage yuv = cc.rgbImg_to_yuv444pImg_R(rgb, color_std, color_range);
        if (!fl.saveYUV444pImage(output_path, yuv))
        {
            std::cerr << "Save YUV444 failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::YUV420P && next_fmt == PixelFormat::RGB24)
    {
        YUV420pImage yuv;
        if (!fl.loadYUV420pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV420 failed\n";
            return -1;
        }
        RGBImage rgb = cc.yuv420pImg_to_rgbImg_R(yuv, color_std, color_range);
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::NV12 && next_fmt == PixelFormat::RGB24)
    {
        NV12Image yuv;
        if (!fl.loadNV12Image(input_path, yuv, width, height))
        {
            std::cerr << "Load NV12 failed\n";
            return -1;
        }
        RGBImage rgb = cc.nv12Img_to_rgbImg_R(yuv, color_std, color_range);
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::NV21 && next_fmt == PixelFormat::RGB24)
    {
        NV21Image yuv;
        if (!fl.loadNV21Image(input_path, yuv, width, height))
        {
            std::cerr << "Load NV21 failed\n";
            return -1;
        }
        RGBImage rgb = cc.nv21Img_to_rgbImg_R(yuv, color_std, color_range);
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::YUV422P && next_fmt == PixelFormat::RGB24)
    {
        YUV422pImage yuv;
        if (!fl.loadYUV422pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV422 failed\n";
            return -1;
        }
        RGBImage rgb = cc.yuv422pImg_to_rgbImg_R(yuv, color_std, color_range);
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::YUV444P && next_fmt == PixelFormat::RGB24)
    {
        YUV444pImage yuv;
        if (!fl.loadYUV444pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV444 failed\n";
            return -1;
        }
        RGBImage rgb = cc.yuv444pImg_to_rgbImg_R(yuv, color_std, color_range);
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else
    {
        std::cerr << "Unsupport convertor\n";
        return -1;
    }

    std::cout << "[DONE]\n";
    return 0;
}