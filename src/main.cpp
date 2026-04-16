#include "../include/color_convertor.h"
#include "../include/utils.h"
#include "../include/file_loader.h"
#include <iostream>

int main(int argc, char *argv[])
{

    if (argc != 8)
    {
        std::cout << "Usage:\n";
        std::cout << "color_convertor input_path output_path width height "
                     "origin_format next_format color_standard\n";
        return -1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    int width = std::stoi(argv[3]);
    int height = std::stoi(argv[4]);
    std::string origin_fmt_str = argv[5];
    std::string next_fmt_str = argv[6];
    std::string color_std_str = argv[7];

    ColorConvertor cc;
    FileLoader fl;

    PixelFormat origin_fmt = Utils::getPixelFormatFromString(origin_fmt_str);
    PixelFormat next_fmt = Utils::getPixelFormatFromString(next_fmt_str);
    ColorStandard color_std = Utils::getColorStandardFromString(color_std_str);

    RGBImage rgb;

    // ================= LOAD INPUT =================
    std::cout << "[INFO] Loading input...\n";

    if (origin_fmt == PixelFormat::RGB24)
    {
        if (!fl.loadRGBImage(input_path, rgb, width, height))
        {
            std::cerr << "Load RGB failed\n";
            return -1;
        }
    }
    else if (origin_fmt == PixelFormat::YUV420P)
    {
        YUV420pImage yuv;
        if (!fl.loadYUV420pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV420 failed\n";
            return -1;
        }
        rgb = cc.yuv420pImg_to_rgbImg(yuv, color_std);
    }
    else if (origin_fmt == PixelFormat::NV12)
    {
        NV12Image nv12;
        if (!fl.loadNV12Image(input_path, nv12, width, height))
        {
            std::cerr << "Load NV12 failed\n";
            return -1;
        }
        rgb = cc.nv12Img_to_rgbImg(nv12, color_std);
    }
    else if (origin_fmt == PixelFormat::NV21)
    {
        NV21Image nv21;
        if (!fl.loadNV21Image(input_path, nv21, width, height))
        {
            std::cerr << "Load NV21 failed\n";
            return -1;
        }
        rgb = cc.nv21Img_to_rgbImg(nv21, color_std);
    }
    else if (origin_fmt == PixelFormat::YUV422P)
    {
        YUV422pImage yuv;
        if (!fl.loadYUV422pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV422 failed\n";
            return -1;
        }
        rgb = cc.yuv422pImg_to_rgbImg(yuv, color_std);
    }
    else if (origin_fmt == PixelFormat::YUV444P)
    {
        YUV444pImage yuv;
        if (!fl.loadYUV444pImage(input_path, yuv, width, height))
        {
            std::cerr << "Load YUV444 failed\n";
            return -1;
        }
        rgb = cc.yuv444pImg_to_rgbImg(yuv, color_std);
    }
    else
    {
        std::cerr << "Unsupported input format\n";
        return -1;
    }

    // ================= CONVERT TO OUTPUT =================
    std::cout << "[INFO] Converting...\n";

    if (next_fmt == PixelFormat::RGB24)
    {
        if (!fl.saveRGBImage(output_path, rgb))
        {
            std::cerr << "Save RGB failed\n";
            return -1;
        }
    }
    else if (next_fmt == PixelFormat::YUV420P)
    {
        auto yuv = cc.rgbImg_to_yuv420pImg(rgb, color_std);
        if (!fl.saveYUV420pImage(output_path, yuv))
        {
            std::cerr << "Save YUV420 failed\n";
            return -1;
        }
    }
    else if (next_fmt == PixelFormat::NV12)
    {
        auto nv12 = cc.rgbImg_to_nv12Img(rgb, color_std);
        if (!fl.saveNV12Image(output_path, nv12))
        {
            std::cerr << "Save NV12 failed\n";
            return -1;
        }
    }
    else if (next_fmt == PixelFormat::NV21)
    {
        auto nv21 = cc.rgbImg_to_nv21Img(rgb, color_std);
        if (!fl.saveNV21Image(output_path, nv21))
        {
            std::cerr << "Save NV21 failed\n";
            return -1;
        }
    }
    else if (next_fmt == PixelFormat::YUV422P)
    {
        auto yuv = cc.rgbImg_to_yuv422pImg(rgb, color_std);
        if (!fl.saveYUV422pImage(output_path, yuv))
        {
            std::cerr << "Save YUV422 failed\n";
            return -1;
        }
    }
    else if (next_fmt == PixelFormat::YUV444P)
    {
        auto yuv = cc.rgbImg_to_yuv444pImg(rgb, color_std);
        if (!fl.saveYUV444pImage(output_path, yuv))
        {
            std::cerr << "Save YUV444 failed\n";
            return -1;
        }
    }
    else
    {
        std::cerr << "Unsupported output format\n";
        return -1;
    }

    std::cout << "[DONE]\n";
    return 0;
}