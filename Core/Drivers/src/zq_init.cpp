//
// Created by fairy on 2025/3/2 19:54.
//
#include "zq_init.h"
// 头文件
#include<zq_sdram.h>
#include<zq_pll.h>

// 函数
void ZQ_Init()
{
    zq::pll::MainPLL::configure<10>();
    ZQ_SDRAM_Init();
}
