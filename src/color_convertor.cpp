#include "../include/color_convertor.h"
#include "../include/utils.h"
#include "../include/file_loader.h"

#include <fstream>
#include <cstring>

// ================= PIXEL CONVERT =================

YUVPixel ColorConvertor::rgb_to_yuv_R(const RGBPixel &rgb,
                                      const ColorStandard &standard,
                                      const ColorRange &range)
{
    Coeff c = getCoeff(standard);

    float R = rgb.r;
    float G = rgb.g;
    float B = rgb.b;

    float Yf = c.Kr * R + c.Kg * G + c.Kb * B;

    float Cb = (B - Yf) / (2.0f * (1.0f - c.Kb));
    float Cr = (R - Yf) / (2.0f * (1.0f - c.Kr));

    float Y, U, V;

    if (range == ColorRange::FULL)
    {
        Y = Yf;
        U = Cb + 128.0f;
        V = Cr + 128.0f;
    }
    else
    {
        // limited range
        Y = Yf * (219.0f / 255.0f) + 16.0f;
        U = Cb * (224.0f / 255.0f) + 128.0f;
        V = Cr * (224.0f / 255.0f) + 128.0f;
    }

    return {
        Utils::clamp(Y),
        Utils::clamp(U),
        Utils::clamp(V)};
}

RGBPixel ColorConvertor::yuv_to_rgb_R(const YUVPixel &yuv,
                                      const ColorStandard &standard,
                                      const ColorRange &range)
{
    Coeff c = getCoeff(standard);

    float Y = yuv.y;
    float U = yuv.u - 128.0f;
    float V = yuv.v - 128.0f;

    float Yf, Cb, Cr;

    if (range == ColorRange::FULL)
    {
        Yf = Y;
        Cb = U;
        Cr = V;
    }
    else
    {
        // LIMITED inverse
        Yf = (Y - 16.0f) * (255.0f / 219.0f);
        Cb = U * (255.0f / 224.0f);
        Cr = V * (255.0f / 224.0f);
    }

    float R = Yf + 2.0f * (1.0f - c.Kr) * Cr;
    float B = Yf + 2.0f * (1.0f - c.Kb) * Cb;
    float G = (Yf - c.Kr * R - c.Kb * B) / c.Kg;

    return {
        Utils::clamp(R),
        Utils::clamp(G),
        Utils::clamp(B)};
}

// ================= YUV444P =================
YUV444pImage ColorConvertor::rgbImg_to_yuv444pImg_R(
    const RGBImage &rgb,
    const ColorStandard &standard,
    const ColorRange &range)
{
    int width = rgb.width;
    int height = rgb.height;
    int size = width * height;

    YUV444pImage yuv;
    yuv.width = width;
    yuv.height = height;

    yuv.yPlane.resize(size);
    yuv.uPlane.resize(size);
    yuv.vPlane.resize(size);

    for (int i = 0; i < size; i++)
    {
        RGBPixel pixel{
            rgb.data[3 * i + 0],
            rgb.data[3 * i + 1],
            rgb.data[3 * i + 2]};

        // ✅ dùng lại pixel function
        YUVPixel yuvPixel = rgb_to_yuv_R(pixel, standard, range);

        yuv.yPlane[i] = yuvPixel.y;
        yuv.uPlane[i] = yuvPixel.u;
        yuv.vPlane[i] = yuvPixel.v;
    }

    return yuv;
}

RGBImage ColorConvertor::yuv444pImg_to_rgbImg_R(
    const YUV444pImage &yuv,
    const ColorStandard &standard,
    const ColorRange &range)
{
    int width = yuv.width;
    int height = yuv.height;
    int size = width * height;

    RGBImage rgb;
    rgb.width = width;
    rgb.height = height;
    rgb.data.resize(size * 3);

    for (int i = 0; i < size; i++)
    {
        YUVPixel yuvPixel{
            yuv.yPlane[i],
            yuv.uPlane[i],
            yuv.vPlane[i]};

        // ✅ dùng lại pixel function
        RGBPixel pixel = yuv_to_rgb_R(yuvPixel, standard, range);

        rgb.data[3 * i + 0] = pixel.r;
        rgb.data[3 * i + 1] = pixel.g;
        rgb.data[3 * i + 2] = pixel.b;
    }

    return rgb;
}

YUV420pImage ColorConvertor::rgbImg_to_yuv420pImg_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range)
{
    // 1. RGB -> YUV444
    YUV444pImage yuv444 = rgbImg_to_yuv444pImg_R(rgbImage, standard, range);

    // 2. Downsample -> 420
    return yuv444_to_yuv420p(yuv444);
}
RGBImage ColorConvertor::yuv420pImg_to_rgbImg_R(const YUV420pImage &yuvImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = yuv420p_to_yuv444(yuvImage);
    return yuv444pImg_to_rgbImg_R(yuv444, standard, range);
}

NV12Image ColorConvertor::rgbImg_to_nv12Img_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = rgbImg_to_yuv444pImg_R(rgbImage, standard, range);
    return yuv444_to_nv12(yuv444);
}
RGBImage ColorConvertor::nv12Img_to_rgbImg_R(const NV12Image &yuvImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = nv12_to_yuv444(yuvImage);
    return yuv444pImg_to_rgbImg_R(yuv444, standard, range);
}

NV21Image ColorConvertor::rgbImg_to_nv21Img_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = rgbImg_to_yuv444pImg_R(rgbImage, standard, range);
    return yuv444_to_nv21(yuv444);
}
RGBImage ColorConvertor::nv21Img_to_rgbImg_R(const NV21Image &yuvImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = nv21_to_yuv444(yuvImage);
    return yuv444pImg_to_rgbImg_R(yuv444, standard, range);
}

YUV422pImage ColorConvertor::rgbImg_to_yuv422pImg_R(const RGBImage &rgbImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = rgbImg_to_yuv444pImg_R(rgbImage, standard, range);
    return yuv444_to_yuv422p(yuv444);
}
RGBImage ColorConvertor::yuv422pImg_to_rgbImg_R(const YUV422pImage &yuvImage, const ColorStandard &standard, const ColorRange &range)
{
    YUV444pImage yuv444 = yuv422p_to_yuv444(yuvImage);
    return yuv444pImg_to_rgbImg_R(yuv444, standard, range);
}

YUV420pImage ColorConvertor::yuv444_to_yuv420p(const YUV444pImage &in)
{
    int w = in.width;
    int h = in.height;

    YUV420pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane; // giữ nguyên

    int w2 = w / 2;
    int h2 = h / 2;

    out.uPlane.resize(w2 * h2);
    out.vPlane.resize(w2 * h2);

    for (int j = 0; j < h2; j++)
    {
        for (int i = 0; i < w2; i++)
        {
            int x = i * 2;
            int y = j * 2;

            int idx0 = y * w + x;
            int idx1 = idx0 + 1;
            int idx2 = idx0 + w;
            int idx3 = idx2 + 1;

            int u_avg = (in.uPlane[idx0] + in.uPlane[idx1] + in.uPlane[idx2] + in.uPlane[idx3]) / 4;
            int v_avg = (in.vPlane[idx0] + in.vPlane[idx1] + in.vPlane[idx2] + in.vPlane[idx3]) / 4;

            out.uPlane[j * w2 + i] = u_avg;
            out.vPlane[j * w2 + i] = v_avg;
        }
    }

    return out;
}

YUV444pImage ColorConvertor::yuv420p_to_yuv444(const YUV420pImage &in)
{
    int w = in.width;
    int h = in.height;

    YUV444pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;

    out.uPlane.resize(w * h);
    out.vPlane.resize(w * h);

    int w2 = w / 2;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int u_idx = (j / 2) * w2 + (i / 2);

            out.uPlane[j * w + i] = in.uPlane[u_idx];
            out.vPlane[j * w + i] = in.vPlane[u_idx];
        }
    }

    return out;
}

NV12Image ColorConvertor::yuv444_to_nv12(const YUV444pImage &in)
{
    int w = in.width;
    int h = in.height;

    NV12Image out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;

    int w2 = w / 2;
    int h2 = h / 2;

    out.uvPlane.resize(w * h / 2);

    for (int j = 0; j < h2; j++)
    {
        for (int i = 0; i < w2; i++)
        {
            int x = i * 2;
            int y = j * 2;

            int idx0 = y * w + x;
            int idx1 = idx0 + 1;
            int idx2 = idx0 + w;
            int idx3 = idx2 + 1;

            int u = (in.uPlane[idx0] + in.uPlane[idx1] + in.uPlane[idx2] + in.uPlane[idx3]) / 4;
            int v = (in.vPlane[idx0] + in.vPlane[idx1] + in.vPlane[idx2] + in.vPlane[idx3]) / 4;

            int uv_idx = (j * w + i * 2);
            out.uvPlane[uv_idx] = u;
            out.uvPlane[uv_idx + 1] = v;
        }
    }

    return out;
}

YUV444pImage ColorConvertor::nv12_to_yuv444(const NV12Image &in)
{
    int w = in.width;
    int h = in.height;

    YUV444pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;
    out.uPlane.resize(w * h);
    out.vPlane.resize(w * h);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int uv_idx = (j / 2) * w + (i & ~1);

            out.uPlane[j * w + i] = in.uvPlane[uv_idx];
            out.vPlane[j * w + i] = in.uvPlane[uv_idx + 1];
        }
    }

    return out;
}

NV21Image ColorConvertor::yuv444_to_nv21(const YUV444pImage &in)
{
    NV21Image out;
    out.width = in.width;
    out.height = in.height;

    out.yPlane = in.yPlane;
    out.vuPlane.resize(in.width * in.height / 2);

    int w = in.width;
    int h = in.height;

    for (int j = 0; j < h / 2; j++)
    {
        for (int i = 0; i < w / 2; i++)
        {
            int x = i * 2, y = j * 2;

            int idx = y * w + x;

            int u = (in.uPlane[idx] + in.uPlane[idx + 1] + in.uPlane[idx + w] + in.uPlane[idx + w + 1]) / 4;
            int v = (in.vPlane[idx] + in.vPlane[idx + 1] + in.vPlane[idx + w] + in.vPlane[idx + w + 1]) / 4;

            int pos = j * w + i * 2;
            out.vuPlane[pos] = v;
            out.vuPlane[pos + 1] = u;
        }
    }

    return out;
}

YUV444pImage ColorConvertor::nv21_to_yuv444(const NV21Image &in)
{
    int w = in.width;
    int h = in.height;

    YUV444pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;
    out.uPlane.resize(w * h);
    out.vPlane.resize(w * h);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int uv_idx = (j / 2) * w + (i & ~1);

            out.uPlane[j * w + i] = in.vuPlane[uv_idx];
            out.vPlane[j * w + i] = in.vuPlane[uv_idx + 1];
        }
    }

    return out;
}

YUV422pImage ColorConvertor::yuv444_to_yuv422p(const YUV444pImage &in)
{
    int w = in.width;
    int h = in.height;

    YUV422pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;

    out.uPlane.resize((w / 2) * h);
    out.vPlane.resize((w / 2) * h);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w / 2; i++)
        {
            int x = i * 2;

            int idx0 = j * w + x;
            int idx1 = idx0 + 1;

            out.uPlane[j * (w / 2) + i] = (in.uPlane[idx0] + in.uPlane[idx1]) / 2;
            out.vPlane[j * (w / 2) + i] = (in.vPlane[idx0] + in.vPlane[idx1]) / 2;
        }
    }

    return out;
}

YUV444pImage ColorConvertor::yuv422p_to_yuv444(const YUV422pImage &in)
{
    int w = in.width;
    int h = in.height;

    YUV444pImage out;
    out.width = w;
    out.height = h;

    out.yPlane = in.yPlane;
    out.uPlane.resize(w * h);
    out.vPlane.resize(w * h);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int src_idx = j * (w / 2) + (i / 2);

            out.uPlane[j * w + i] = in.uPlane[src_idx];
            out.vPlane[j * w + i] = in.vPlane[src_idx];
        }
    }

    return out;
}

YUV444pImage ColorConvertor::convert_to_444(PixelFormat origin_fmt, const std::string &input_path, int width, int height, FileLoader &fl)
{
    if (origin_fmt == PixelFormat::YUV444P)
    {
        YUV444pImage yuv;
        if (!fl.loadYUV444pImage(input_path, yuv, width, height))
            throw std::runtime_error("Load YUV444 failed");
        return yuv;
    }
    else if (origin_fmt == PixelFormat::YUV420P)
    {
        YUV420pImage yuv;
        if (!fl.loadYUV420pImage(input_path, yuv, width, height))
            throw std::runtime_error("Load YUV420 failed");
        return ColorConvertor::yuv420p_to_yuv444(yuv);
    }
    else if (origin_fmt == PixelFormat::NV12)
    {
        NV12Image yuv;
        if (!fl.loadNV12Image(input_path, yuv, width, height))
            throw std::runtime_error("Load NV12 failed");
        return ColorConvertor::nv12_to_yuv444(yuv);
    }
    else if (origin_fmt == PixelFormat::NV21)
    {
        NV21Image yuv;
        if (!fl.loadNV21Image(input_path, yuv, width, height))
            throw std::runtime_error("Load NV21 failed");
        return ColorConvertor::nv21_to_yuv444(yuv);
    }
    else if (origin_fmt == PixelFormat::YUV422P)
    {
        YUV422pImage yuv;
        if (!fl.loadYUV422pImage(input_path, yuv, width, height))
            throw std::runtime_error("Load YUV422 failed");
        return ColorConvertor::yuv422p_to_yuv444(yuv);
    }

    throw std::runtime_error("Unsupported origin format for to_444");
}

void ColorConvertor::convert_from_444(const YUV444pImage &yuv444, PixelFormat next_fmt, const std::string &output_path, FileLoader &fl)
{
    if (next_fmt == PixelFormat::YUV444P)
    {
        if (!fl.saveYUV444pImage(output_path, yuv444))
            throw std::runtime_error("Save YUV444 failed");
    }
    else if (next_fmt == PixelFormat::YUV420P)
    {
        YUV420pImage out = ColorConvertor::yuv444_to_yuv420p(yuv444);
        if (!fl.saveYUV420pImage(output_path, out))
            throw std::runtime_error("Save YUV420 failed");
    }
    else if (next_fmt == PixelFormat::NV12)
    {
        NV12Image out = ColorConvertor::yuv444_to_nv12(yuv444);
        if (!fl.saveNV12Image(output_path, out))
            throw std::runtime_error("Save NV12 failed");
    }
    else if (next_fmt == PixelFormat::NV21)
    {
        NV21Image out = ColorConvertor::yuv444_to_nv21(yuv444);
        if (!fl.saveNV21Image(output_path, out))
            throw std::runtime_error("Save NV21 failed");
    }
    else if (next_fmt == PixelFormat::YUV422P)
    {
        YUV422pImage out = ColorConvertor::yuv444_to_yuv422p(yuv444);
        if (!fl.saveYUV422pImage(output_path, out))
            throw std::runtime_error("Save YUV422 failed");
    }
    else
    {
        throw std::runtime_error("Unsupported next format for from_444");
    }
}