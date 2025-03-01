//
// Created by fairy on 2025/3/2 00:15.
//
#include "sdram.h"
// 头文件

#define EBSR (*(ioport volatile uint16_t*)0x6c00)
#define EGCR (*(ioport volatile uint16_t*)0x800)
#define EMIRST (*(ioport volatile uint16_t*)0x801)
// #define EMIBE (*(ioport volatile uint16_t*)0x802)
#define CE01 (*(ioport volatile uint16_t*)0x803)
// #define CE02 (*(ioport volatile uint16_t*)0x804)
// #define CE03 (*(ioport volatile uint16_t*)0x805)
#define CE11 (*(ioport volatile uint16_t*)0x806)
// #define CE12 (*(ioport volatile uint16_t*)0x807)
// #define CE13 (*(ioport volatile uint16_t*)0x808)
#define CE21 (*(ioport volatile uint16_t*)0x809)
// #define CE22 (*(ioport volatile uint16_t*)0x80A)
// #define CE23 (*(ioport volatile uint16_t*)0x80B)
#define CE31 (*(ioport volatile uint16_t*)0x80C)
// #define CE32 (*(ioport volatile uint16_t*)0x80D)
// #define CE33 (*(ioport volatile uint16_t*)0x80E)
#define SDC1 (*(ioport volatile uint16_t*)0x80F)
// #define SDPER (*(ioport volatile uint16_t*)0x810)
// #define SDCTN (*(ioport volatile uint16_t*)0x811)
#define INIT (*(ioport volatile uint16_t*)0x812)
#define SDC2 (*(ioport volatile uint16_t*)0x813)

// 函数
void sdram_init()
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