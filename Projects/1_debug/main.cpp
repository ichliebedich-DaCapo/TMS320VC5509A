//
// Created by fairy on 2025/3/1 21:22.
//
//-------------------------------相关头文件------------------------------------//
#include<app_conf.h>
#include <zq_conf.h>
#include <zq_pll.h>
#include<zq_gpio.h>
#include <zq_init.h>
#include <zq_interrupt.h>
#include<zq_timer.h>
#if PROGRAM_INDEX == 1
#include <stdio.h>
#include <zq_adc.h>
#include<led.h>
#include <dip.h>
#include<zq_gui.h>
#include<oled.h>
#include<zq_gui.h>

//-----------------------------------主程序------------------------------------//
uint32_t count = 0;

int main()
{
    ZQ_Init();

    // ====== 外设初始化 ======
    // 理论   分频      实际
    // 100K → 2000 → 732
    // 10K  → 20000 → 732
    // 1M   → 200 →
    // arr,psc
    // 9,39     183
    // 9,79     183
    // 9,4      293
    // 1,1      732
    // 9,10     488
    // 9,9      732
    // 4,4      293
    // 2,4      293

    // psc,arr
    // 2,2      488
    // 4,2      293
    // 4,5      293
    // 9,2      146
    // 14,5     97
    // 14,99    97
    bsp::LED::clear();
    zq::timer::Timer0::init(TIM_FREQ_192M_to_2K);
    zq::timer::Timer0::start_IT();



    // ======无限循环======
    while (true)
    {
        if (count >= 500) // 5000000
        {
            count = 0;
            bsp::LED::toggle(bsp::led::pin::LED_1);
        }
    }
}

//---------------------------------子程序--------------------------------------//
extern "C"
{
    void interrupt Timer0_ISR()
    {
        ++count;
    }
}

#endif
