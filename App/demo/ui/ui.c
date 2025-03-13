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


typedef struct
{
    uint8_t waveform[128];
    uint8_t w_index;
    uint8_t timebase;
    uint8_t trigger_level;
    uint8_t vertical_scale;
} Oscilloscope;


// 静态界面元素缓存
Oscilloscope osc;


/* 在文件作用域声明所有变量 */
static uint8_t last_y = 0;
static uint8_t x = 0;

void ui_init()
{


    gui_clear();
    //
    // gui_draw_hline(32, 64, 0, 1);
    // gui_draw_hline(32, 64, 2, 1);
    // gui_draw_hline(32, 64, 13, 1);
    // gui_draw_vline(0, 5, 20, 1);

    // gui_draw_rect(0, 0, 48, 32, 1);
    // gui_draw_circle(24,24,16,1);
}


void ui_handler()
{
#ifdef SIMULATOR
    /* 更新波形数据 */
    static uint16_t phase = 0;
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = (uint8_t) (32 * (sin(phase * 0.1 + x * 0.2) + 1));
    }

    /*图形系统*/
    gui_clear();
    for (x = 0; x < 127; x++)
    {
        gui_draw_line(x, 63-osc.waveform[x], x+1, 63-osc.waveform[x+1], 1);
    }


    phase++;


#endif
}
