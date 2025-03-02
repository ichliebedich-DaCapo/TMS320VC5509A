//
// Created by fairy on 2025/3/2 00:15.
//
#ifndef SDRAM_H
#define SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zq_conf.h>

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

/**
 * @brief 初始化SDRAM
 * @note LED、DIP等片外外设使用前必须初始化SRAM
 */
void ZQ_SDRAM_Init();


#ifdef __cplusplus
}
#endif
#endif //SDRAM_H
