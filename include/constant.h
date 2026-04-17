#ifndef COLOR_CONVERT_CONSTANT_H
#define COLOR_CONVERT_CONSTANT_H

#include <iostream>

enum class PixelFormat
{
    RGB24,
    YUV420P,
    NV12,
    NV21,
    YUV422P,
    YUV444P
};

enum class ColorStandard
{
    BT601,
    BT709,
    BT2020
};

enum class ColorRange {
    FULL,
    LIMITED
};

struct Coeff {
    float Kr;
    float Kg;
    float Kb;
};

inline Coeff getCoeff(ColorStandard cs) {
    switch (cs) {
        case ColorStandard::BT601:  return {0.299f,  0.587f, 0.114f};
        case ColorStandard::BT709:  return {0.2126f, 0.7152f, 0.0722f};
        case ColorStandard::BT2020: return {0.2627f, 0.6780f, 0.0593f};
    }
    return {0.2126f, 0.7152f, 0.0722f};
}

#endif