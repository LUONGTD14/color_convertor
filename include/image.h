#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>

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

#endif