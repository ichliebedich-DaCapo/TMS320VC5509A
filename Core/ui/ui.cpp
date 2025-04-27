//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include<zq_gui.h>
#include<math.h>
#include<zq_font.h>

// *************先这样，后续会迁移到ui.h里*************

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
    // 初始化界面
    void Render::screen()
    {
        uint16_t data =0x55;
        for (int i=0;i<32;++i)
        {
            for (int j=0;j<4;++j)
                Tools::write_data(j, i, data);
            data =~data;
        }

        // 生成的字库
        Tools::draw_string("LCD",0,47,Font::fonts_12x8);
        Tools::draw_string("DONE",20,47,Font::fonts_12x8);

        // 手动字库
        Tools::draw_string("L", 0, 55, Font::fonts_10x8);
        Tools::draw_string("C", 6, 55, Font::fonts_10x8);
        Tools::draw_string("D", 12, 55, Font::fonts_10x8);
        Tools::draw_string("D", 21, 55, Font::fonts_10x8);
        Tools::draw_string("O", 27, 55, Font::fonts_10x8);
        Tools::draw_string("N", 33, 55, Font::fonts_10x8);
        Tools::draw_string("E", 39, 55, Font::fonts_10x8);

        Tools::draw_line(64,0,95,63);// 左半边折线
        Tools::draw_line(127,0,96,63);// 右半边折线
    }


    void Render::draw()
    {


        // /*界面开发……*/
        // static uint16_t phase=0;
        // if (uiDelay.is_timeout())
        // {
        //     // 清屏
        // Tools::clear();
        // //     phase = (++phase) & 127;
        // //     Tools::draw_vline(phase, 10, 20);
        // // }
        // // // 设置刷新标志
        // Flag::render::set();
    }
}
