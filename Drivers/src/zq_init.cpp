//
// Created by fairy on 2025/3/2 19:54.
//
#include "zq_init.h"
// 头文件
#include<zq_sdram.h>
#include<zq_pll.h>
#include <zq_interrupt.h>
#include <zq_systick.h>
// 函数
void ZQ_Init()
{
    zq::pll::MainPLL::configure<16>();// 配置主PLL为16*12=192MHz
    ZQ_SDRAM_Init();// 初始化SDRAM
    zq::isr::init();// 初始化中断
    zq::systick::init();
}
