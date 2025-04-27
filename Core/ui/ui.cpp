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
constexpr uint16_t  MAX_VALUE = 100;
namespace constexpr_math {
    constexpr float pow(float x, int n) {
        return n == 0 ? 1.0f : x * pow(x, n-1);
    }

    constexpr float exp(float x) {
        // 处理负指数
        if(x < 0) return 1.0f / exp(-x);

        // 增加泰勒展开项数到15项
        float result = 1.0f;
        float term = 1.0f;
        for(int n=1; n<15; ++n) {
            term *= x/n;
            result += term;
        }
        return result;
    }
}
constexpr std::array<float, 1024> generate_spectrum_data()
{
    std::array<float, 1024> data{};

    // 主频峰参数
    constexpr float peak_freq = 128.0f;  // 中心位置左移
    constexpr float peak_width = 100.0f; // 增大宽度
    constexpr float peak_amplitude = 1.0f; // 主峰幅值

    // 噪声参数
    constexpr float noise_level = 0.05f; // 基础噪声水平

    for (size_t i = 0; i < data.size(); ++i)
    {
        // 高斯主峰
        float gaussian = peak_amplitude *
            constexpr_math::exp(-constexpr_math::pow((i - peak_freq)/peak_width, 2)/2);
        // 伪随机噪声（基于索引的确定性随机）
        float noise = noise_level *
                      (0.5f * std::sin(i * 3.14f / 180.0f) +
                       0.3f * std::cos(i * 0.78f) +
                       0.2f * std::sin(i * 2.2f));

        // 叠加信号
        data[i] = gaussian + noise;

        data[i]*=MAX_VALUE;

        // 限制幅值范围
        data[i] = data[i] < 0.0f ? 0.0f : data[i];
        data[i] = data[i] > MAX_VALUE*1.5 ? MAX_VALUE*1.5 : data[i];

    }
    return data;
}
constexpr auto spectrum_data = generate_spectrum_data();
// ====================== 仿真数据 =======================


#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define PLOT_ORIGIN_X 20  // 纵轴标签预留空间
#define PLOT_ORIGIN_Y OLED_HEIGHT-8 // 横轴标签位置
#define PLOT_WIDTH 101    // 可绘制区域宽度
#define PLOT_HEIGHT 48    // 可绘制区域高度
#define MAX_FREQ 48000

struct AxisRange
{
    float freq_start; // 起始频率
    float freq_end; // 结束频率
    float amp_min; // 最小幅值
    float amp_max; // 最大幅值
};


// 界面开发
namespace GUI
{
    void drawCoordinateSystem(const AxisRange &range)
    {
        // 绘制坐标轴
        Tools::draw_vline(PLOT_ORIGIN_X, 0, PLOT_ORIGIN_Y); // 纵轴
        Tools::draw_hline(PLOT_ORIGIN_Y, PLOT_ORIGIN_X, OLED_WIDTH); // 横轴

        // 横轴刻度（示例：5等分）
        for (int i = 0; i <= 5; ++i)
        {
            uint16_t x = PLOT_ORIGIN_X + i * PLOT_WIDTH / 5;
            Tools::draw_vline(x, PLOT_ORIGIN_Y - 3, PLOT_ORIGIN_Y);
            // 频率标签
            char buf[8];
            const float freq = range.freq_start + (range.freq_end - range.freq_start) * i / 5;
            sprintf(buf, "%.0f", freq);
            Tools::draw_string(buf, x - 4, PLOT_ORIGIN_Y + 2, Font::fonts_10x10);
        }

        // 纵轴刻度（示例：4等分）
        for (int i = 0; i <= 4; ++i)
        {
            uint16_t y = PLOT_ORIGIN_Y - i * PLOT_HEIGHT / 4;
            Tools::draw_hline(y, PLOT_ORIGIN_X - 3, PLOT_ORIGIN_X);
            // 幅值标签
            char buf[8];
            float amp = range.amp_min + (range.amp_max - range.amp_min) * i / 4;
            sprintf(buf, "%.1f", amp);
            Tools::draw_string(buf, 0, y - 4, Font::fonts_10x10);
        }
    }

    void drawSpectrum(const float *spectrum, const AxisRange &range)
    {
        const int totalPoints = 1024;
        const int displayPoints = PLOT_WIDTH;
        const int pointsPerSegment = totalPoints / displayPoints;

        float prevY = PLOT_ORIGIN_Y; // 初始位置

        for (int i = 0; i < displayPoints; ++i)
        {
            // 峰值抽取
            float maxVal = 0;
            int start = i * pointsPerSegment;
            int end = (i == displayPoints - 1) ? totalPoints : start + pointsPerSegment;

            for (int j = start; j < end; ++j)
            {
                if (spectrum[j] > maxVal) maxVal = spectrum[j];
            }

            // 坐标映射
            uint16_t x = PLOT_ORIGIN_X + i;
            uint16_t y = PLOT_ORIGIN_Y - static_cast<uint16_t>(
                             (maxVal - range.amp_min) / (range.amp_max - range.amp_min) * PLOT_HEIGHT);

            // 线性插值绘制
            Tools::draw_line(x - 1, prevY, x, y);
            prevY = y;
        }
    }

    enum ZoomFactor { ZOOM_1X, ZOOM_2X, ZOOM_4X };

    AxisRange applyZoom(AxisRange current, ZoomFactor zoom, bool isHorizontal)
    {
        if (isHorizontal)
        {
            float centerFreq = (current.freq_start + current.freq_end) / 2;
            float newRange = (current.freq_end - current.freq_start) / (1 << zoom);
            current.freq_start = centerFreq - newRange / 2;
            current.freq_end = centerFreq + newRange / 2;

            // 边界检查
            if (current.freq_start < 0)
            {
                current.freq_end -= current.freq_start;
                current.freq_start = 0;
            }
        }
        else
        {
            float centerAmp = (current.amp_min + current.amp_max) / 2;
            float newRange = (current.amp_max - current.amp_min) / (1 << zoom);
            current.amp_min = centerAmp - newRange / 2;
            current.amp_max = centerAmp + newRange / 2;
        }
        return current;
    }

    AxisRange panFrequency(AxisRange current, int direction, float panStep)
    {
        float range = current.freq_end - current.freq_start;
        float delta = range * panStep * direction;

        current.freq_start += delta;
        current.freq_end += delta;

        // 边界检查
        if (current.freq_start < 0)
        {
            current.freq_end -= current.freq_start;
            current.freq_start = 0;
        }
        else if (current.freq_end > MAX_FREQ)
        {
            // 假设有最大频率常量
            current.freq_start -= (current.freq_end - MAX_FREQ);
            current.freq_end = MAX_FREQ;
        }
        return current;
    }

    class SpectrumAnalyzer
    {
        static inline AxisRange currentRange;
        static inline ZoomFactor hZoom;
        static inline ZoomFactor vZoom;

    public:
        static void updateDisplay(const float *spectrum)
        {
            Tools::clear();
            drawCoordinateSystem(currentRange);
            drawSpectrum(spectrum, currentRange);
            // 此处应添加刷新屏幕缓冲区的代码
        }

        static void handleZoom(bool isHorizontal, bool zoomIn)
        {
            ZoomFactor &zoom = isHorizontal ? hZoom : vZoom;
            zoom = static_cast<ZoomFactor>(zoom + (zoomIn ? 1 : -1));
            currentRange = applyZoom(currentRange, zoom, isHorizontal);
        }

        static void handlePan(int direction)
        {
            currentRange = panFrequency(currentRange, direction, 0.2f);
        }
    };

    // 在数据预处理阶段进行幅值归一化
    void preProcessSpectrum(float *spectrum, int size)
    {
        static float maxValCache = 0;
        for (int i = 0; i < size; ++i)
        {
            if (spectrum[i] > maxValCache) maxValCache = spectrum[i];
            else spectrum[i] = maxValCache * 0.95; // 峰值保持
        }
    }
}


namespace GUI
{
    // 初始化界面
    void Render::screen()
    {
        uiDelay.start(800);
    }


    void Render::draw()
    {
        // preProcessSpectrum(spectrum_data.data(),  1024);
        SpectrumAnalyzer::updateDisplay(spectrum_data.data());

        /*界面开发……*/
        if (uiDelay.is_timeout()) {}
    }
}
