#include "../include/file_loader.h"
#include "../include/color_convertor.h"
#include "../include/utils.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

bool FileLoader::readRawFile(const std::string &filename, std::vector<uint8_t> &buffer, size_t expectedSize)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        return false;

    buffer.resize(expectedSize);
    file.read(reinterpret_cast<char *>(buffer.data()), expectedSize);
    return true;
}

bool FileLoader::writeRawFile(const std::string &filename, const std::vector<uint8_t> &buffer)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
        return false;

    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    return true;
}

// ================= RGB IMAGE =================
bool FileLoader::loadRGBImage(const std::string &filename,
                              RGBImage &img, int w, int h)
{
    img.width = w;
    img.height = h;
    return readRawFile(filename, img.data, w * h * 3);
}

bool FileLoader::saveRGBImage(const std::string &filename,
                              const RGBImage &img)
{
    return writeRawFile(filename, img.data);
}

// ================= YUV444P =================
bool FileLoader::loadYUV444pImage(const std::string &filename,
                                  YUV444pImage &img, int w, int h)
{

    img.width = w;
    img.height = h;

    size_t size = w * h;

    std::vector<uint8_t> buffer;
    if (!readRawFile(filename, buffer, size * 3))
        return false;

    img.yPlane.assign(buffer.begin(), buffer.begin() + size);
    img.uPlane.assign(buffer.begin() + size, buffer.begin() + 2 * size);
    img.vPlane.assign(buffer.begin() + 2 * size, buffer.end());

    return true;
}

bool FileLoader::saveYUV444pImage(const std::string &filename,
                                  const YUV444pImage &img)
{

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), img.yPlane.begin(), img.yPlane.end());
    buffer.insert(buffer.end(), img.uPlane.begin(), img.uPlane.end());
    buffer.insert(buffer.end(), img.vPlane.begin(), img.vPlane.end());

    return writeRawFile(filename, buffer);
}

// ================= YUV420P =================
bool FileLoader::loadYUV420pImage(const std::string &filename,
                                  YUV420pImage &img, int w, int h)
{
    img.width = w;
    img.height = h;

    size_t ySize = w * h;
    size_t uvSize = (w / 2) * (h / 2);

    std::vector<uint8_t> buffer;
    if (!readRawFile(filename, buffer, ySize + 2 * uvSize))
        return false;

    img.yPlane.assign(buffer.begin(), buffer.begin() + ySize);
    img.uPlane.assign(buffer.begin() + ySize, buffer.begin() + ySize + uvSize);
    img.vPlane.assign(buffer.begin() + ySize + uvSize, buffer.end());

    return true;
}

bool FileLoader::saveYUV420pImage(const std::string &filename,
                                  const YUV420pImage &img)
{

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), img.yPlane.begin(), img.yPlane.end());
    buffer.insert(buffer.end(), img.uPlane.begin(), img.uPlane.end());
    buffer.insert(buffer.end(), img.vPlane.begin(), img.vPlane.end());

    return writeRawFile(filename, buffer);
}

// ================= NV12 =================
bool FileLoader::loadNV12Image(const std::string &filename,
                               NV12Image &img, int w, int h)
{

    img.width = w;
    img.height = h;

    size_t ySize = w * h;
    size_t uvSize = w * h / 2;

    std::vector<uint8_t> buffer;
    if (!readRawFile(filename, buffer, ySize + uvSize))
        return false;

    img.yPlane.assign(buffer.begin(), buffer.begin() + ySize);
    img.uvPlane.assign(buffer.begin() + ySize, buffer.end());

    return true;
}

bool FileLoader::saveNV12Image(const std::string &filename,
                               const NV12Image &img)
{

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), img.yPlane.begin(), img.yPlane.end());
    buffer.insert(buffer.end(), img.uvPlane.begin(), img.uvPlane.end());

    return writeRawFile(filename, buffer);
}

// ================= NV21 =================
bool FileLoader::loadNV21Image(const std::string &filename,
                               NV21Image &img, int w, int h)
{

    img.width = w;
    img.height = h;

    size_t ySize = w * h;
    size_t uvSize = w * h / 2;

    std::vector<uint8_t> buffer;
    if (!readRawFile(filename, buffer, ySize + uvSize))
        return false;

    img.yPlane.assign(buffer.begin(), buffer.begin() + ySize);
    img.vuPlane.assign(buffer.begin() + ySize, buffer.end());

    return true;
}

bool FileLoader::saveNV21Image(const std::string &filename,
                               const NV21Image &img)
{

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), img.yPlane.begin(), img.yPlane.end());
    buffer.insert(buffer.end(), img.vuPlane.begin(), img.vuPlane.end());

    return writeRawFile(filename, buffer);
}

// ================ YUV422P =================
bool FileLoader::loadYUV422pImage(const std::string &filename,
                                  YUV422pImage &img, int w, int h)
{

    img.width = w;
    img.height = h;

    size_t ySize = w * h;
    size_t uvSize = w * h / 2;

    std::vector<uint8_t> buffer;
    if (!readRawFile(filename, buffer, ySize + 2 * uvSize))
        return false;

    img.yPlane.assign(buffer.begin(), buffer.begin() + ySize);
    img.uPlane.assign(buffer.begin() + ySize, buffer.begin() + ySize + uvSize);
    img.vPlane.assign(buffer.begin() + ySize + uvSize, buffer.end());

    return true;
}

bool FileLoader::saveYUV422pImage(const std::string &filename,
                                  const YUV422pImage &img)
{

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), img.yPlane.begin(), img.yPlane.end());
    buffer.insert(buffer.end(), img.uPlane.begin(), img.uPlane.end());
    buffer.insert(buffer.end(), img.vPlane.begin(), img.vPlane.end());

    return writeRawFile(filename, buffer);
}