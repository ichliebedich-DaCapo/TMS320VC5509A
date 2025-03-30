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

    // GUI_Object::draw_hline(0,32,1,1);
    //     GUI_Object::draw_vline(0,32,64,1);
    //     // GUI_Object::draw_line(0,0,32,32,1);
    //     // GUI_Object::draw_circle(32,32,18,1);
    //     // GUI_Object::draw_rect(16,16,32,32,1);
    //     GUI_Object::fill_rect(10,10,10,8,1);
    //
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
}
