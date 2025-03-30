//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include "u8g2.h"
#include<gui.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
// 头文件


// 函数


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
static char buf[10];
static uint16_t cache[1024];

void ui_init()
{
    /* 网格系统 */
    u8g2_SetDrawColor(&u8g2, 1);

    /* 垂直网格 */
    for (x = 0; x < 128; x += 16)
    {
        u8g2_DrawVLine(&u8g2, x, 8, 48);
    }

    /* 水平网格 */
    for (x = 0; x < 64; x += 16)
    {
        // 修改循环变量名避免冲突
        u8g2_DrawHLine(&u8g2, 0, x + 8, 128);
    }

    /* 固定文字 */
    u8g2_SetFont(&u8g2, u8g2_font_3x5im_mr);
    u8g2_DrawStr(&u8g2, 2, 64, "CH1");
    u8g2_DrawStr(&u8g2, 112, 64, "RUN");

    /* 初始化波形缓冲区 */
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = 32; // 初始居中
    }
    // 发送缓冲区数据
    memcpy(cache, u8g2.tile_buf_ptr, 1024);
}


void ui_handler()
{
    /* 更新波形数据 */
    static uint16_t phase = 0;
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = (uint16_t) ( 110 * (sin(phase * 0.1 + x * 0.2)+1.1));
    }
    phase++;


    /* 开始绘制流程 */
    // 接收缓冲区数据
    memcpy(u8g2.tile_buf_ptr, cache, 1024);

    /* 动态波形绘制 */
    u8g2_SetDrawColor(&u8g2, 1);
    last_y = osc.waveform[0]*48/255.0f;
    for (x = 1; x < 128; x++)
    {
        u8g2_DrawLine(&u8g2,x - 1, 56 - last_y,x,56 - osc.waveform[x]*48/255.0f);
        last_y = osc.waveform[x]*48/255.0f;
    }

    /* 实时参数显示 */
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    sprintf(buf, "%3dHz", 1000);
    u8g2_DrawStr(&u8g2, 50, 8, buf);

    u8g2_SendBuffer(&u8g2);
}
