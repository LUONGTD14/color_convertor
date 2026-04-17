#ifndef UTILS_H
#define UTILS_H

#include "../include/constant.h"

#include <iostream>

class Utils
{
public:
    Utils(/* args */) {};
    ~Utils() {};

    static uint8_t clamp(float value);
    static PixelFormat getPixelFormatFromString(const std::string &formatStr);
    static ColorStandard getColorStandardFromString(const std::string &standardStr);
    static ColorRange getColorRangeFromString(const std::string &rangeStr);
};

#endif
