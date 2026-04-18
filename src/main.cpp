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
    YUV444pImage yuv = cc.convert_to_444(origin_fmt, input_path, width, height, fl);
    cc.convert_from_444(yuv, next_fmt, output_path, fl);

    std::cout << "[DONE]\n";
    return 0;
}