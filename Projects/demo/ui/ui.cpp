//
// Created by fairy on 2025/3/3 12:14.
//
#include "ui.h"
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
        // 边界线
        Tools::draw_hline(1,101,0);
        Tools::draw_hline(1,101,63);
        Tools::draw_vline(0,0,63);
        Tools::draw_vline(101,0,63);



        for (uint16_t i=1;i<5;++i)
        {
            // 绘制竖线刻度
            // 总共100列，分成4个，那么步进为100/(4+1)=20,说明可整分
            Tools::draw_vline(20*i,1,2);

            // 绘制横线刻度
            // 总共64-2-2行，最后减去的那两行是竖线刻度的长度，那么步进为60/(4+1)=12
            Tools::draw_hline(99,100,3+12*i);
        }

    }


    void Render::draw()
    {
        /* 更新频谱数据 */


    }
}