//
// Created by fairy on 2025/3/2 00:15.
//
#include <zq_sdram.h>
// 头文件



// 函数
void ZQ_SDRAM_Init()
{
    EBSR = 0x221;
    EGCR = 0x220;
    CE01 = 0x3000;
    CE11 = 0x1fff;
    CE21 = 0x1fff;
    CE31 = 0x1fff;
    EMIRST = 0;
    SDC1 = 0x5958;
    SDC2 = 0x38F;
    INIT = 0;
}