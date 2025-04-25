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
    // 初始化界面
    void Render::screen() {}

    uint16_t tick = 0;

    void Render::draw()
    {
        // 清屏
        Tools::clear();

        /*界面开发……*/

        // 设置刷新标志
        Flag::render::set();
    }
}
