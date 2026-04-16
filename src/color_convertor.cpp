#include "../include/color_convertor.h"
#include "../include/utils.h"
#include <fstream>
#include <cstring>

// ================= MATRIX SELECT =================

static const float (*getRGB2YUV(ColorStandard s))[3]
{
    static ColorMatrixBT601 bt601;
    static ColorMatrixBT709 bt709;
    static ColorMatrixBT2020 bt2020;

    switch (s)
    {
    case ColorStandard::BT601:
        return bt601.rgb_to_yuv;
    case ColorStandard::BT709:
        return bt709.rgb_to_yuv;
    case ColorStandard::BT2020:
        return bt2020.rgb_to_yuv;
    }
    return bt601.rgb_to_yuv;
}

static const float (*getYUV2RGB(ColorStandard s))[3]
{
    static ColorMatrixBT601 bt601;
    static ColorMatrixBT709 bt709;
    static ColorMatrixBT2020 bt2020;

    switch (s)
    {
    case ColorStandard::BT601:
        return bt601.yuv_to_rgb;
    case ColorStandard::BT709:
        return bt709.yuv_to_rgb;
    case ColorStandard::BT2020:
        return bt2020.yuv_to_rgb;
    }
    return bt601.yuv_to_rgb;
}

// ================= PIXEL CONVERT =================

YUVPixel ColorConvertor::rgb_to_yuv(const RGBPixel &rgb,
                                    const ColorStandard &standard)
{
    auto m = getRGB2YUV(standard);

    float y = m[0][0] * rgb.r + m[0][1] * rgb.g + m[0][2] * rgb.b;
    float u = m[1][0] * rgb.r + m[1][1] * rgb.g + m[1][2] * rgb.b + 128.0f;
    float v = m[2][0] * rgb.r + m[2][1] * rgb.g + m[2][2] * rgb.b + 128.0f;

    return {Utils::clamp(y), Utils::clamp(u), Utils::clamp(v)};
}

RGBPixel ColorConvertor::yuv_to_rgb(const YUVPixel &yuv,
                                    const ColorStandard &standard)
{
    auto m = getYUV2RGB(standard);

    float u = yuv.u - 128.0f;
    float v = yuv.v - 128.0f;

    float r = m[0][0] * yuv.y + m[0][1] * u + m[0][2] * v;
    float g = m[1][0] * yuv.y + m[1][1] * u + m[1][2] * v;
    float b = m[2][0] * yuv.y + m[2][1] * u + m[2][2] * v;

    return {Utils::clamp(r), Utils::clamp(g), Utils::clamp(b)};
}

// ================= YUV420P =================

YUV420pImage ColorConvertor::rgbImg_to_yuv420pImg(
    const RGBImage &rgb, const ColorStandard &standard)
{

    YUV420pImage out;
    int w = rgb.width;
    int h = rgb.height;

    out.width = w;
    out.height = h;

    out.yPlane.resize(w * h);
    out.uPlane.resize(w * h / 4);
    out.vPlane.resize(w * h / 4);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int idx = (j * w + i) * 3;
            RGBPixel p{rgb.data[idx], rgb.data[idx + 1], rgb.data[idx + 2]};
            auto yuv = rgb_to_yuv(p, standard);

            out.yPlane[j * w + i] = yuv.y;

            if (j % 2 == 0 && i % 2 == 0)
            {
                int uv_idx = (j / 2) * (w / 2) + (i / 2);
                out.uPlane[uv_idx] = yuv.u;
                out.vPlane[uv_idx] = yuv.v;
            }
        }
    }
    return out;
}

RGBImage ColorConvertor::yuv420pImg_to_rgbImg(
    const YUV420pImage &yuv, const ColorStandard &standard)
{

    RGBImage out;
    int w = yuv.width;
    int h = yuv.height;

    out.width = w;
    out.height = h;
    out.data.resize(w * h * 3);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int y_idx = j * w + i;
            int uv_idx = (j / 2) * (w / 2) + (i / 2);

            YUVPixel p;
            p.y = yuv.yPlane[y_idx];
            p.u = yuv.uPlane[uv_idx];
            p.v = yuv.vPlane[uv_idx];

            auto rgb = yuv_to_rgb(p, standard);

            int idx = (j * w + i) * 3;
            out.data[idx] = rgb.r;
            out.data[idx + 1] = rgb.g;
            out.data[idx + 2] = rgb.b;
        }
    }
    return out;
}

// ================= NV12 =================

NV12Image ColorConvertor::rgbImg_to_nv12Img(
    const RGBImage &rgb, const ColorStandard &standard)
{

    NV12Image out;
    int w = rgb.width;
    int h = rgb.height;

    out.width = w;
    out.height = h;

    out.yPlane.resize(w * h);
    out.uvPlane.resize(w * h / 2);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int idx = (j * w + i) * 3;
            RGBPixel p{rgb.data[idx], rgb.data[idx + 1], rgb.data[idx + 2]};
            auto yuv = rgb_to_yuv(p, standard);

            out.yPlane[j * w + i] = yuv.y;

            if (j % 2 == 0 && i % 2 == 0)
            {
                int uv_idx = (j / 2) * w + i;
                out.uvPlane[uv_idx] = yuv.u;
                out.uvPlane[uv_idx + 1] = yuv.v;
            }
        }
    }
    return out;
}

RGBImage ColorConvertor::nv12Img_to_rgbImg(
    const NV12Image &nv12, const ColorStandard &standard)
{

    RGBImage out;
    int w = nv12.width;
    int h = nv12.height;

    out.width = w;
    out.height = h;
    out.data.resize(w * h * 3);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int y_idx = j * w + i;
            int uv_idx = (j / 2) * w + (i & ~1);

            YUVPixel p;
            p.y = nv12.yPlane[y_idx];
            p.u = nv12.uvPlane[uv_idx];
            p.v = nv12.uvPlane[uv_idx + 1];

            auto rgb = yuv_to_rgb(p, standard);

            int idx = (j * w + i) * 3;
            out.data[idx] = rgb.r;
            out.data[idx + 1] = rgb.g;
            out.data[idx + 2] = rgb.b;
        }
    }
    return out;
}

// ================= NV21 =================

NV21Image ColorConvertor::rgbImg_to_nv21Img(
    const RGBImage &rgb, const ColorStandard &standard)
{

    NV21Image out;
    int w = rgb.width;
    int h = rgb.height;

    out.width = w;
    out.height = h;

    out.yPlane.resize(w * h);
    out.vuPlane.resize(w * h / 2);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int idx = (j * w + i) * 3;
            RGBPixel p{rgb.data[idx], rgb.data[idx + 1], rgb.data[idx + 2]};
            auto yuv = rgb_to_yuv(p, standard);

            out.yPlane[j * w + i] = yuv.y;

            if (j % 2 == 0 && i % 2 == 0)
            {
                int uv_idx = (j / 2) * w + i;
                out.vuPlane[uv_idx] = yuv.v;
                out.vuPlane[uv_idx + 1] = yuv.u;
            }
        }
    }
    return out;
}

RGBImage ColorConvertor::nv21Img_to_rgbImg(
    const NV21Image &nv21, const ColorStandard &standard)
{

    RGBImage out;
    int w = nv21.width;
    int h = nv21.height;

    out.width = w;
    out.height = h;
    out.data.resize(w * h * 3);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int y_idx = j * w + i;
            int uv_idx = (j / 2) * w + (i & ~1);

            YUVPixel p;
            p.y = nv21.yPlane[y_idx];
            p.v = nv21.vuPlane[uv_idx];
            p.u = nv21.vuPlane[uv_idx + 1];

            auto rgb = yuv_to_rgb(p, standard);

            int idx = (j * w + i) * 3;
            out.data[idx] = rgb.r;
            out.data[idx + 1] = rgb.g;
            out.data[idx + 2] = rgb.b;
        }
    }
    return out;
}

// ================= YUV422P =================
YUV422pImage ColorConvertor::rgbImg_to_yuv422pImg(
    const RGBImage &rgb, const ColorStandard &standard)
{

    YUV422pImage out;
    int w = rgb.width;
    int h = rgb.height;

    out.width = w;
    out.height = h;

    out.yPlane.resize(w * h);
    out.uPlane.resize(w * h / 2);
    out.vPlane.resize(w * h / 2);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int idx = (j * w + i) * 3;
            RGBPixel p{rgb.data[idx], rgb.data[idx + 1], rgb.data[idx + 2]};
            auto yuv = rgb_to_yuv(p, standard);

            int y_idx = j * w + i;
            out.yPlane[y_idx] = yuv.y;

            // subsample theo chiều ngang
            if (i % 2 == 0)
            {
                int uv_idx = j * (w / 2) + (i / 2);
                out.uPlane[uv_idx] = yuv.u;
                out.vPlane[uv_idx] = yuv.v;
            }
        }
    }
    return out;
}

RGBImage ColorConvertor::yuv422pImg_to_rgbImg(
    const YUV422pImage &yuv, const ColorStandard &standard)
{

    RGBImage out;
    int w = yuv.width;
    int h = yuv.height;

    out.width = w;
    out.height = h;
    out.data.resize(w * h * 3);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int y_idx = j * w + i;
            int uv_idx = j * (w / 2) + (i / 2);

            YUVPixel p;
            p.y = yuv.yPlane[y_idx];
            p.u = yuv.uPlane[uv_idx];
            p.v = yuv.vPlane[uv_idx];

            auto rgb = yuv_to_rgb(p, standard);

            int idx = y_idx * 3;
            out.data[idx] = rgb.r;
            out.data[idx + 1] = rgb.g;
            out.data[idx + 2] = rgb.b;
        }
    }
    return out;
}

// ================= YUV444P =================
YUV444pImage ColorConvertor::rgbImg_to_yuv444pImg(
    const RGBImage &rgb, const ColorStandard &standard)
{

    YUV444pImage out;
    int w = rgb.width;
    int h = rgb.height;

    out.width = w;
    out.height = h;

    out.yPlane.resize(w * h);
    out.uPlane.resize(w * h);
    out.vPlane.resize(w * h);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int idx = (j * w + i) * 3;
            RGBPixel p{rgb.data[idx], rgb.data[idx + 1], rgb.data[idx + 2]};

            auto yuv = rgb_to_yuv(p, standard);

            int pos = j * w + i;
            out.yPlane[pos] = yuv.y;
            out.uPlane[pos] = yuv.u;
            out.vPlane[pos] = yuv.v;
        }
    }
    return out;
}

RGBImage ColorConvertor::yuv444pImg_to_rgbImg(
    const YUV444pImage &yuv, const ColorStandard &standard)
{

    RGBImage out;
    int w = yuv.width;
    int h = yuv.height;

    out.width = w;
    out.height = h;
    out.data.resize(w * h * 3);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {

            int pos = j * w + i;

            YUVPixel p;
            p.y = yuv.yPlane[pos];
            p.u = yuv.uPlane[pos];
            p.v = yuv.vPlane[pos];

            auto rgb = yuv_to_rgb(p, standard);

            int idx = pos * 3;
            out.data[idx] = rgb.r;
            out.data[idx + 1] = rgb.g;
            out.data[idx + 2] = rgb.b;
        }
    }
    return out;
}