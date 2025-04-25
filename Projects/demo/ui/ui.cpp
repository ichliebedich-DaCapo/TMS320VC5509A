//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include<zq_gui.h>
#include<math.h>
#include<zq_font.h>

typedef struct
{
    uint16_t waveform[128];
    uint16_t w_index;
    uint16_t timebase;
    uint16_t trigger_level;
    uint16_t vertical_scale;
} Oscilloscope;


// 静态界面元素缓存
Oscilloscope osc;

// 界面开发
namespace GUI
{
    class SpectrumAnalyzer
    {
    public:
        void startAnalysis(); // 开始频谱分析
        void stopAnalysis(); // 停止分析
    };
}


namespace GUI
{
    struct FontType
    {
        const char *name;
        const char data;
    };

    //牛顿
    template<uint16_t steps>
    void draw_newton_interpolation(Point p0, Point p1, Point p2, Point p3)
    {
        // ================= 参数预处理 =================
        const float s0 = 0.0f; // 对应p0的参数位置
        const float s1 = 1.0f; // 对应p1的参数位置
        const float s2 = 2.0f; // 对应p2的参数位置
        const float s3 = 3.0f; // 对应p3的参数位置

        // ================= 计算X方向差商 =================
        // 一阶差商
        const float f01_x = (p1.x - p0.x) / (s1 - s0);
        const float f12_x = (p2.x - p1.x) / (s2 - s1);
        const float f23_x = (p3.x - p2.x) / (s3 - s2);

        // 二阶差商
        const float f012_x = (f12_x - f01_x) / (s2 - s0);
        const float f123_x = (f23_x - f12_x) / (s3 - s1);

        // 三阶差商
        const float f0123_x = (f123_x - f012_x) / (s3 - s0);

        // ================= 计算Y方向差商 =================
        // 一阶差商
        const float f01_y = (p1.y - p0.y) / (s1 - s0);
        const float f12_y = (p2.y - p1.y) / (s2 - s1);
        const float f23_y = (p3.y - p2.y) / (s3 - s2);

        // 二阶差商
        const float f012_y = (f12_y - f01_y) / (s2 - s0);
        const float f123_y = (f23_y - f12_y) / (s3 - s1);

        // 三阶差商
        const float f0123_y = (f123_y - f012_y) / (s3 - s0);

        // ================= 生成插值点 =================
        for (uint16_t i = 0; i <= steps; ++i)
        {
            const float t = static_cast<float>(i) / steps;
            const float s = s0 + t * (s3 - s0); // 关键修改：s ∈ [s0,s3]

            // 牛顿插值多项式计算
            const float dx = s - s0;
            const float dy_x = f01_x * dx
                             + f012_x * dx * (s - s1)
                             + f0123_x * dx * (s - s1) * (s - s2);

            const float dy_y = f01_y * dx
                             + f012_y * dx * (s - s1)
                             + f0123_y * dx * (s - s1) * (s - s2);

            // 坐标计算和像素写入保持不变
            const float x = p0.x + dy_x;
            const float y = p0.y + dy_y;
            Tools::write_pixel(static_cast<uint16_t>(x + 0.5f),
                              static_cast<uint16_t>(y + 0.5f));
        }
    }


    // 初始化界面
    void Render::screen()
    {

    }

    uint16_t tick=0;
    void Render::draw()
    {
        // /* 清除绘制区域的背景色 */
        tick++;
        if (tick>800)
        {
            tick=0;
            Tools::clear();
            static unsigned int phase = 0;
            ++phase;
            int xsin, ysin[123];
            for (xsin = 0; xsin < 123; xsin = xsin + 1)
            {
                ysin[xsin] = (20 * sin(2 * 3.1415926 * (1 + phase) / 20.0 * xsin)) + 30;
            }

            for (xsin = 0; xsin < 120; ++xsin) //将sin函数值存入数组
            {
                Point point[4];
                for (int i = 0; i < 4; ++i)
                {
                    point[i].x = xsin + i;
                    point[i].y = ysin[xsin + i];
                }

                //牛顿
                draw_newton_interpolation<10>(point[0], point[1], point[2], point[3]);

            }
            Flag::render::set();
        }
    }
}
