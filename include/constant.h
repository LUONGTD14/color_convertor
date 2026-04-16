#ifndef COLOR_CONVERT_CONSTANT_H
#define COLOR_CONVERT_CONSTANT_H

#include <iostream>

struct ColorMatrixBT601
{
    float yuv_to_rgb[3][3] = {
        {1.0f, 0.0f, 1.4075f},
        {1.0f, -0.3455f, -0.7169f},
        {1.0f, 1.779f, 0.0f}};
    float rgb_to_yuv[3][3] = {
        {0.299f, 0.587f, 0.114f},
        {-0.169f, -0.331f, 0.5f},
        {0.5f, -0.419f, -0.081f}};
};

struct ColorMatrixBT709
{
    float yuv_to_rgb[3][3] = {
        {1.0f, 0.0f, 1.5748f},
        {1.0f, -0.1868f, -0.4680f},
        {1.0f, 1.856f, 0.0f}};
    float rgb_to_yuv[3][3] = {
        {0.2129f, 0.7154f, 0.072f},
        {-0.1145f, -0.33855f, 0.5f},
        {0.5f, -0.4543f, -0.0457f}};
};

struct ColorMatrixBT2020
{
    float yuv_to_rgb[3][3] = {
        {1.0f, 0.0f, 1.4746f},
        {1.0f, -0.1645f, -0.5713f},
        {1.0f, 1.8814f, 0.0f}};
    float rgb_to_yuv[3][3] = {
        {0.2627f, 0.6780f, 0.0593f},
        {-0.1396f, -0.3604f, 0.5f},
        {0.5f, -0.4598f, -0.0402f}};
};

// Enum cho định dạng pixel
enum class PixelFormat
{
    RGB24,
    YUV420P,
    NV12,
    NV21,
    YUV422P,
    YUV444P
};

// Enum cho tiêu chuẩn màu
enum class ColorStandard
{
    BT601,
    BT709,
    BT2020
};

#endif