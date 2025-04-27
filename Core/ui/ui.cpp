//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include<zq_gui.h>
#include<math.h>
#include<zq_font.h>
#include <zq_systick.h>
#include<simulator.hpp>
zq::systick::AsyncDelay<get_tick> uiDelay;
// *************先这样，后续会迁移到ui.h里*************
// ====================== 仿真数据 =======================
#include <array>
#include <cmath>
// 添加自定义数学函数实现避免报错
constexpr uint16_t MAX_VALUE = 100;

namespace constexpr_math
{
    constexpr float pow(float x, int n)
    {
        return n == 0 ? 1.0f : x * pow(x, n - 1);
    }

    constexpr float exp(float x)
    {
        // 处理负指数
        if (x < 0) return 1.0f / exp(-x);

        // 增加泰勒展开项数到15项
        float result = 1.0f;
        float term = 1.0f;
        for (int n = 1; n < 15; ++n)
        {
            term *= x / n;
            result += term;
        }
        return result;
    }
}

constexpr std::array<float, 1024> generate_spectrum_data()
{
    std::array<float, 1024> data{};

    // 主频峰参数
    constexpr float peak_freq = 128.0f; // 中心位置左移
    constexpr float peak_width = 100.0f; // 增大宽度
    constexpr float peak_amplitude = 1.0f; // 主峰幅值

    // 噪声参数
    constexpr float noise_level = 0.05f; // 基础噪声水平

    for (size_t i = 0; i < data.size(); ++i)
    {
        // 高斯主峰
        float gaussian = peak_amplitude *
                         constexpr_math::exp(-constexpr_math::pow((i - peak_freq) / peak_width, 2) / 2);
        // 伪随机噪声（基于索引的确定性随机）
        float noise = noise_level *
                      (0.5f * std::sin(i * 3.14f / 180.0f) +
                       0.3f * std::cos(i * 0.78f) +
                       0.2f * std::sin(i * 2.2f));

        // 叠加信号
        data[i] = gaussian + noise;

        data[i] *= MAX_VALUE;

        // 限制幅值范围
        data[i] = data[i] < 0.0f ? 0.0f : data[i];
        data[i] = data[i] > MAX_VALUE * 1.5 ? MAX_VALUE * 1.5 : data[i];
    }
    return data;
}

constexpr auto spectrum_data = generate_spectrum_data();
// ====================== 仿真数据 =======================
#include <ui.h>

// 界面开发
namespace GUI
{
    float SpectrumDisplay::startFreq = 0.0f;
    float SpectrumDisplay::endFreq = 24000.0f;
    float SpectrumDisplay::minAmpl = 0.0f;
    float SpectrumDisplay::maxAmpl = 1.0f;
    SpectrumDisplay::ZoomLevel SpectrumDisplay::currentZoom = ZOOM_1X;
    const float SpectrumDisplay::SAMPLE_RATE = 48000.0f;

    void SpectrumDisplay::Init()
    {
        startFreq = 0.0f;
        endFreq = 24000.0f;
        minAmpl = 0.0f;
        maxAmpl = 1.0f;
        currentZoom = ZOOM_1X;
    }

    void SpectrumDisplay::DrawAxes()
    {
        // 清空绘图区域
        Tools::clear(AXIS_LEFT, 0, AXIS_LEFT + PLOT_WIDTH, AXIS_BOTTOM);

        DrawHorizontalAxis();
        DrawVerticalAxis();
    }

    void SpectrumDisplay::DrawHorizontalAxis()
    {
        // 绘制横轴
        Tools::draw_hline(AXIS_BOTTOM, AXIS_LEFT, AXIS_LEFT + PLOT_WIDTH);

        // 主要刻度（每20像素）
        for (uint16_t x = AXIS_LEFT; x <= AXIS_LEFT + PLOT_WIDTH; x += 20)
        {
            Tools::draw_vline(x, AXIS_BOTTOM - 3, AXIS_BOTTOM);

            float freq = startFreq + (endFreq - startFreq) *
                         (x - AXIS_LEFT) / (float) PLOT_WIDTH;
            char buf[8];
            sprintf(buf, "%.0f", freq / 1000);

            // 调整标签位置防止溢出
            uint16_t xPos = x - 5;
            if (x < AXIS_LEFT + 10) xPos = AXIS_LEFT;
            else if (x > AXIS_LEFT + PLOT_WIDTH - 10) xPos = AXIS_LEFT + PLOT_WIDTH - 10;

            Tools::draw_string(buf, xPos, AXIS_BOTTOM + 2, Font::fonts_10x10);
        }
    }

    void SpectrumDisplay::DrawVerticalAxis()
    {
        // 绘制纵轴
        Tools::draw_vline(AXIS_LEFT, AXIS_BOTTOM - PLOT_HEIGHT, AXIS_BOTTOM);

        // 主要刻度（每10像素）
        for (uint16_t y = AXIS_BOTTOM - PLOT_HEIGHT; y <= AXIS_BOTTOM; y += 12)
        {
            Tools::draw_hline(AXIS_LEFT, AXIS_LEFT + 3, y);

            float ampl = minAmpl + (maxAmpl - minAmpl) *
                         (AXIS_BOTTOM - y) / (float) PLOT_HEIGHT;
            char buf[6];
            sprintf(buf, "%.1f", ampl);

            uint16_t xPos = AXIS_LEFT - 15;
            if (ampl >= 10.0f) xPos -= 5;

            Tools::draw_string(buf, xPos, y - 5, Font::fonts_10x10);
        }
    }

    void SpectrumDisplay::DrawSpectrum(const float spectrum[])
    {
        const int totalBins = FFT_SIZE / 2;
        const float freqPerBin = SAMPLE_RATE / FFT_SIZE;

        int prevY = AXIS_BOTTOM;
        for (uint16_t x = 0; x < PLOT_WIDTH; ++x)
        {
            // 计算当前列对应的频率范围
            float relPos = x / (float) (PLOT_WIDTH - 1);
            float start = startFreq + relPos * (endFreq - startFreq);
            float end = start + (endFreq - startFreq) / (PLOT_WIDTH - 1);

            // 转换为索引范围
            int startIdx = floor(start / freqPerBin);
            int endIdx = ceil(end / freqPerBin);
            startIdx = std::max(0, std::min(startIdx, totalBins - 1));
            endIdx = std::max(0, std::min(endIdx, totalBins - 1));

            // 抽取峰值
            float maxVal = 0.0f;
            for (int i = startIdx; i <= endIdx; ++i)
            {
                if (spectrum[i] > maxVal) maxVal = spectrum[i];
            }

            // 映射到Y坐标
            int y = AXIS_BOTTOM - static_cast<int>((maxVal - minAmpl) / (maxAmpl - minAmpl) * PLOT_HEIGHT);
            y = std::max(AXIS_BOTTOM - PLOT_HEIGHT, std::min(y, static_cast<int>(AXIS_BOTTOM)));

            // 绘制连线
            if (x > 0)
            {
                Tools::draw_line(
                    AXIS_LEFT + x - 1, prevY,
                    AXIS_LEFT + x, y
                );
            }
            prevY = y;
        }
    }

    void SpectrumDisplay::SetZoom(ZoomLevel level)
    {
        float center = (startFreq + endFreq) / 2;
        float newWidth = 24000.0f / (1 << (level * 2)); // 示例缩放逻辑

        startFreq = center - newWidth / 2;
        endFreq = center + newWidth / 2;

        // 边界检查
        if (startFreq < 0)
        {
            endFreq -= startFreq;
            startFreq = 0;
        }
        if (endFreq > 24000)
        {
            startFreq -= (endFreq - 24000);
            endFreq = 24000;
        }

        currentZoom = level;
    }

    void SpectrumDisplay::Pan(bool moveLeft)
    {
        float range = endFreq - startFreq;
        float shift = range * 0.1f;

        if (moveLeft)
        {
            startFreq = std::max(0.0f, startFreq - shift);
            endFreq = startFreq + range;
        }
        else
        {
            endFreq = std::min(24000.0f, endFreq + shift);
            startFreq = endFreq - range;
        }
    }

    void SpectrumDisplay::SetAmplitudeRange(float min, float max)
    {
        minAmpl = min;
        maxAmpl = max;
    }
}


namespace GUI
{
    // 初始化界面
    void Render::screen()
    {
        uiDelay.start(800);
        SpectrumDisplay::Init();
        SpectrumDisplay::DrawAxes();
        SpectrumDisplay::SetAmplitudeRange(0,100);

    }


    void Render::draw()
    {
        // 清屏
        Tools::clear();

        SpectrumDisplay::DrawAxes();
        SpectrumDisplay::DrawSpectrum(spectrum_data.data());

        // 刷新
        Flag::render::set();
    }
}
