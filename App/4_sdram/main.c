//
// Created by fairy on 2025/3/1 23:45.
//
// 头文件
#include<app_conf.h>
#if PROGRAM_INDEX == 4

#include<zq_delay.h>
#include<led.h>
#include <dip.h>
#include<sdram.h>
#include<zq_pll.h>



// 流水灯测试
void led_test();

// 拨码控制led测试
void dip_test();

// 函数
int main()
{
    ZQ_PLL_Init();
    sdram_init();

    while (1)
    {
        led_test();
    }
}


void led_test()
{
    int16_t i;
    for (i = 0; i < 4; ++i)
    {
        led_on(1 << i); // 正向开启
        delay(256);
    }
    for (i = 3; i >= 0; --i)
    {
        led_off(1 << i); // 反向关闭
        delay(256);
    }
}


void dip_test()
{
    leds_set(dips_get(DIP_1 | DIP_2 | DIP_3 | DIP_4));
}

#endif
