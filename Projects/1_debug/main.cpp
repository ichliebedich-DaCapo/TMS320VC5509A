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
    bsp::LED::clear();
    zq::timer::Timer0::init(TIM_FREQ_200M_to_100K);
    zq::isr::start_timer();



    // ======无限循环======
    while (true)
    {
        // 如果如我预期是200MHz，那么count应该接近

        if (count >= 200) // 5000000
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
