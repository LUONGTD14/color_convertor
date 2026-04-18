#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "image.h"

#include <vector>
#include <string>

class FileLoader
{
public:
    FileLoader() = default;
    ~FileLoader() = default;

    bool readRawFile(const std::string &filename, std::vector<uint8_t> &buffer, size_t expectedSize);
    bool writeRawFile(const std::string &filename, const std::vector<uint8_t> &buffer);

    bool loadRGBImage(const std::string &filename, RGBImage &image, int width, int height);
    bool saveRGBImage(const std::string &filename, const RGBImage &image);

    bool loadYUV444pImage(const std::string &filename, YUV444pImage &image, int width, int height);
    bool saveYUV444pImage(const std::string &filename, const YUV444pImage &image);

    bool loadYUV420pImage(const std::string &filename, YUV420pImage &image, int width, int height);
    bool saveYUV420pImage(const std::string &filename, const YUV420pImage &image);

    bool loadNV12Image(const std::string &filename, NV12Image &image, int width, int height);
    bool saveNV12Image(const std::string &filename, const NV12Image &image);

    bool loadNV21Image(const std::string &filename, NV21Image &image, int width, int height);
    bool saveNV21Image(const std::string &filename, const NV21Image &image);

    bool loadYUV422pImage(const std::string &filename, YUV422pImage &image, int width, int height);
    bool saveYUV422pImage(const std::string &filename, const YUV422pImage &image);
};

#endif