#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "../include/constant.h"

class Utils
{
public:
    Utils(/* args */) {};
    ~Utils() {};

    static uint8_t clamp(float value);
    static PixelFormat getPixelFormatFromString(const std::string &formatStr);
    static ColorStandard getColorStandardFromString(const std::string &standardStr);
};

#endif
