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

    struct FontType{
        const char* name;
        const char data;
    };


    // 初始化界面
    void Render::screen()
    {
        // 边界线
        Tools::draw_hline(1, 101, 0);
        Tools::draw_hline(1, 101, 63);
        Tools::draw_vline(0, 0, 63);
        Tools::draw_vline(101, 0, 63);

        for (uint16_t i = 1; i < 5; ++i)
        {
            // 绘制竖线刻度
            // 总共100列，分成4个，那么步进为100/(4+1)=20,说明可整分
            Tools::draw_vline(20 * i, 1, 2);

            // 绘制横线刻度
            // 总共64-2-2行，最后减去的那两行是竖线刻度的长度，那么步进为60/(4+1)=12
            Tools::draw_hline(99, 100, 3 + 12 * i);
        }

        // Tools::draw_char("中",12,12,Font::font_18x18);
        // Tools::draw_char("2",30,12,Font::font_18x18);

        // Tools::draw_string("中2",12,12,Font::font_18x18);
        //Tools::draw_string("未曾",5,12,Font::fonts_16x16);

    }


    void Render::draw()
    {
        /* 清除绘制区域的背景色 */
        Tools::clear(0, 7, 1, 100); // 清除绘制区域的背景色

        /* 重新绘制清除的边界 */
        Tools::draw_hline(1, 101, 0);
        Tools::draw_hline(1, 101, 63);
        for (uint16_t i = 1; i < 5; ++i)
            Tools::draw_vline(20 * i, 1, 2);

        /* 更新频谱数据 */
        static uint16_t phase = 0;

        int prev_x = 0, prev_y = 0;
        float offset =0;
        for (int x = 1; x <= 100; x++)
        {
            // 计算当前点的y值
            const float y_val = 32.5f + 29.5f * sin((phase+(offset++))*(2 * 3.14159) / 100); // 中心32.5，振幅29.5，范围3~62
            const int y = static_cast<int>(y_val + 0.5f); // 四舍五入到整数

            // 处理第一个点
            if (x == 1)
            {
                prev_x = x;
                prev_y = y;
            }
            else
            {
                // 绘制从前一个点到当前点的直线
                Tools::draw_line(prev_x, prev_y, x, y);
                prev_x = x;
                prev_y = y;
            }
        }
        ++phase;

        // 模拟ADC数据等待过程
        static uint32_t tick =0;
        if ((tick++)%3==0)
            Flag::render::set();
    }
}
