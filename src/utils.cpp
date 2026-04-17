#include "../include/utils.h"
#include <math.h>

uint8_t Utils::clamp(float v)
{
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return static_cast<uint8_t>(round(v));
}

PixelFormat Utils::getPixelFormatFromString(const std::string &s)
{
    if (s == "rgb")
        return PixelFormat::RGB24;
    if (s == "yuv420p")
        return PixelFormat::YUV420P;
    if (s == "nv12")
        return PixelFormat::NV12;
    if (s == "nv21")
        return PixelFormat::NV21;
    if (s == "yuv422p")
        return PixelFormat::YUV422P;
    if (s == "yuv444p")
        return PixelFormat::YUV444P;
    throw std::runtime_error("Unknown pixel format");
}

ColorStandard Utils::getColorStandardFromString(const std::string &s)
{
    if (s == "bt601")
        return ColorStandard::BT601;
    if (s == "bt709")
        return ColorStandard::BT709;
    if (s == "bt2020")
        return ColorStandard::BT2020;
    throw std::runtime_error("Unknown color standard");
}

ColorRange Utils::getColorRangeFromString(const std::string &r)
{
    if (r == "limited")
        return ColorRange::LIMITED;
    if (r == "full")
        return ColorRange::FULL;
    throw std::runtime_error("Unknown color range");
}