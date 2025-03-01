//
// Created by fairy on 2025/3/2 00:15.
//
#ifndef SDRAM_H
#define SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zq_conf.h>

/**
 * @brief 初始化SDRAM
 * @note LED、DIP等片外外设使用前必须初始化SRAM
 */
void sdram_init();


#ifdef __cplusplus
}
#endif
#endif //SDRAM_H
