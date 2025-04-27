//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include<zq_gui.h>
#include<math.h>
#include<zq_font.h>
#include <zq_systick.h>
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

zq::systick::AsyncDelay uiDelay;

namespace GUI
{
    // 初始化界面
    void Render::screen()
    {
        Tools::draw_string("你是我未曾",10,20,Font::fonts_16x16);
        uiDelay.start(133);
    }


    void Render::draw()
    {


        // /*界面开发……*/
        // static uint16_t phase=0;
        // if (uiDelay.is_timeout())
        // {
        //     // 清屏
        Tools::clear();
        //     phase = (++phase) & 127;
        //     Tools::draw_vline(phase, 10, 20);
        // }
        // // 设置刷新标志
        Flag::render::set();
    }
}
