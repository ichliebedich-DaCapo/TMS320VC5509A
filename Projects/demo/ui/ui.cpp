//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include<gui.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
// 头文件


// 函数
namespace ui
{
    WaveformView<3, 2, 100, 50,34> waveform;
}


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


void ui_init()
{
    ui::waveform.create();


}


void ui_handler()
{
#ifdef SIMULATOR
    /* 更新波形数据 */
    static uint32_t time = 0;
    ++time;
    if (time>=8)
    {
        time = 0;
        static uint16_t phase = 0;
        const uint16_t value = (100 * (sin(phase * 0.17 ) + 1.1));
        phase++;
        ui::waveform.set_next_value(value);
    }



#endif
    ui::waveform.draw();
}
