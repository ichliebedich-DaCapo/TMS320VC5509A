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

    template<uint16_t steps>
     void draw_lagrange_interpolation(Point p0, Point p1, Point p2, Point p3) {
        // 参数预处理
        const float s0 = 0.0f; // p0的参数位置
        const float s1 = 1.0f; // p1的参数位置
        const float s2 = 2.0f; // p2的参数位置
        const float s3 = 3.0f; // p3的参数位置

        // 生成插值点
        for (uint16_t i = 0; i <= steps; ++i) {
            const float t = static_cast<float>(i) / steps; // t ∈ [0,1]
            const float s = s1 + t * (s2 - s1);           // 映射到[s1, s2]区间

            // 计算拉格朗日基函数
            const float L0 = ((s - s1) * (s - s2) * (s - s3)) / ((s0 - s1) * (s0 - s2) * (s0 - s3));
            const float L1 = (s * (s - s2) * (s - s3)) / ((s1 - s0) * (s1 - s2) * (s1 - s3));
            const float L2 = (s * (s - s1) * (s - s3)) / ((s2 - s0) * (s2 - s1) * (s2 - s3));
            const float L3 = (s * (s - s1) * (s - s2)) / ((s3 - s0) * (s3 - s1) * (s3 - s2));

            // 计算插值点的坐标
            const float x = p0.x * L0 + p1.x * L1 + p2.x * L2 + p3.x * L3;
            const float y = p0.y * L0 + p1.y * L1 + p2.y * L2 + p3.y * L3;

            // oled_draw(static_cast<uint16_t>(y + 0.5f), static_cast<uint16_t>(x + 0.5f), ledpoint); // 四舍五入
            Tools::write_pixel(static_cast<uint16_t>(x + 0.5f),static_cast<uint16_t>(y + 0.5f));
        }
    }
    // 初始化界面
    void Render::screen()
    {
        // // 边界线
        // Tools::draw_hline(1, 101, 0);
        // Tools::draw_hline(1, 101, 63);
        // Tools::draw_vline(0, 0, 63);
        // Tools::draw_vline(101, 0, 63);
        //
        // for (uint16_t i = 1; i < 5; ++i)
        // {
        //     // 绘制竖线刻度
        //     // 总共100列，分成4个，那么步进为100/(4+1)=20,说明可整分
        //     Tools::draw_vline(20 * i, 1, 2);
        //
        //     // 绘制横线刻度
        //     // 总共64-2-2行，最后减去的那两行是竖线刻度的长度，那么步进为60/(4+1)=12
        //     Tools::draw_hline(99, 100, 3 + 12 * i);
        // }
        //
        // // Tools::draw_char("中",12,12,Font::font_18x18);
        // // Tools::draw_char("2",30,12,Font::font_18x18);
        //
        //  Tools::draw_string("中2",12,35,Font::fonts_18x18);
        // Tools::draw_string("未曾",5,12,Font::fonts_16x16);
        //拉格朗日

        int xsin, ysin[128];
        for(xsin = 3;xsin<123;xsin=xsin+1)
        {
            ysin[xsin]=(20*sin(2*3.1415926/20*xsin))+30;;
        }

        Point point[4];
        for(xsin = 3;xsin<121;xsin=xsin+1)               //将sin函数值存入数组
        {
            for(int i=0;i<4;++i)
            {
                point[i].x=xsin+i;
                point[i].y=ysin[xsin+i];

            }

            //牛顿
            // draw_newton_interpolation<100>(point[0],point[1],point[2],point[3]);
            draw_lagrange_interpolation<50>(point[0],point[1],point[2],point[3]);
            //拉格朗日
            //               draw_lagrange_interpolation<50>(point[0],point[1],point[2],point[3]);
            //线性
            //               draw_linear_interpolation<20>(point[0],point[1]);
        }

    }


    void Render::draw()
    {
        // /* 清除绘制区域的背景色 */
        // Tools::clear(0, 7, 1, 100); // 清除绘制区域的背景色
        //
        // /* 重新绘制清除的边界 */
        // Tools::draw_hline(1, 101, 0);
        // Tools::draw_hline(1, 101, 63);
        // for (uint16_t i = 1; i < 5; ++i)
        //     Tools::draw_vline(20 * i, 1, 2);
        //
        // /* 更新频谱数据 */
        // static uint16_t phase = 0;
        //
        // int prev_x = 0, prev_y = 0;
        // float offset =0;
        // for (int x = 1; x <= 100; x++)
        // {
        //     // 计算当前点的y值
        //     const float y_val = 32.5f + 29.5f * sin((phase+(offset++))*(2 * 3.14159) / 100); // 中心32.5，振幅29.5，范围3~62
        //     const int y = static_cast<int>(y_val + 0.5f); // 四舍五入到整数
        //
        //     // 处理第一个点
        //     if (x == 1)
        //     {
        //         prev_x = x;
        //         prev_y = y;
        //     }
        //     else
        //     {
        //         // 绘制从前一个点到当前点的直线
        //         Tools::draw_line(prev_x, prev_y, x, y);
        //         prev_x = x;
        //         prev_y = y;
        //     }
        // }
        // ++phase;
        //
        // // 模拟ADC数据等待过程
        // static uint32_t tick =0;
        // if ((tick++)%3==0)
        //     Flag::render::set();
    }
}
