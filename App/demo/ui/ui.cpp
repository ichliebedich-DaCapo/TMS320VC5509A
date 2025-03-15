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
    WaveformView<100,10,10,20,16> waveform;
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


/* 在文件作用域声明所有变量 */
static uint16_t last_y = 0;
static uint16_t x = 0;

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
    static uint16_t phase = 0;
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = static_cast<uint16_t>(32 * (sin(phase * 0.1 + x * 0.2) + 1));
    }

    /*图形系统*/
    // gui_clear();
    // for (x = 0; x < 127; x++)
    // {
    //     gui_draw_line(x, 63-osc.waveform[x], x+1, 63-osc.waveform[x+1], 1);
    // }


    phase++;


#endif
}
