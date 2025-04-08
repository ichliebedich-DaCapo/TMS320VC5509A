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

// 界面开发
namespace GUI
{

   class SpectrumAnalyzer {
    public:
        void startAnalysis(); // 开始频谱分析
        void stopAnalysis();  // 停止分析
    };

}




namespace GUI
{
    // 初始化界面
    void Render::init()
    {


    }


    void Render::draw()
    {
        /* 更新频谱数据 */


    }
}