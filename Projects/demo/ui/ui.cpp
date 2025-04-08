//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include<zq_gui.h>
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





namespace GUI
{
    void Render::init()
    {
        /* 初始化波形数据 */

    }


    void Render::draw()
    {
        /* 更新波形数据 */
        static uint32_t time = 0;
        ++time;
        if (time>=8)
        {
            time = 0;
            static uint16_t phase = 0;
            const uint16_t value = (100 * (sin(phase * 0.17 ) + 1.1));
            phase++;

        }
    }
}