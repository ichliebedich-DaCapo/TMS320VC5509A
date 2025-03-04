//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include <u8g2.h>
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
static char buf[10];

void ui_init()
{
    /* 网格系统 */
    u8g2_SetDrawColor(&u8g2, 1);

    /* 垂直网格 */
    for (x = 0; x < 128; x += 16)
    {
        u8g2_DrawVLine(&u8g2, x, 8, 56);
    }

    /* 水平网格 */
    for (x = 0; x < 64; x += 16)
    {
        // 修改循环变量名避免冲突
        u8g2_DrawHLine(&u8g2, 0, x + 8, 128);
    }

    /* 固定文字 */
    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&u8g2, 2, 63, "CH1");
    u8g2_DrawStr(&u8g2, 110, 63, "RUN");

    /* 初始化波形缓冲区 */
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = 32; // 初始居中
    }

    u8g2_SendBuffer(&u8g2);
    u8g2_SetAutoPageClear(&u8g2, 1);
}


void ui_handler()
{


    /* 更新波形数据 */
    static uint16_t phase = 0;
    for (x = 0; x < 128; x++)
    {
        osc.waveform[x] = 32 + (uint8_t) (30 * sin(phase * 0.1 + x * 0.2));
    }
    phase++;

    /* 开始绘制流程 */
    // u8g2_ClearBuffer(&u8g2);
    u8g2_FirstPage(&u8g2);
    do {
    /* 绘制静态元素 */
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawHLine(&u8g2, 0, 8, 128);

    /* 动态波形绘制 */
    u8g2_SetDrawColor(&u8g2, 1);
    last_y = osc.waveform[0];
    for (x = 1; x < 128; x++)
    {
        u8g2_DrawLine(&u8g2,
                      x - 1,
                      63 - last_y, // Y轴翻转
                      x,
                      63 - osc.waveform[x]);
        last_y = osc.waveform[x];
    }

    /* 实时参数显示 */
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    sprintf(buf, "%3dHz", 1000);
    u8g2_DrawStr(&u8g2, 50, 8, buf);

    } while (u8g2_NextPage(&u8g2));

    // u8g2_SendBuffer(&u8g2);
}
