//
// Created by fairy on 2025/3/2 00:07.
//
#ifndef DIPS_H
#define DIPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zq_conf.h>

// 定义拨码开关控制寄存器地址和寄存器类型
#define DIP_BASE (*(unsigned int *)0x400002)

typedef enum
{
    DIP_1 = 0x0001,
    DIP_2 = 0x0002,
    DIP_3 = 0x0004,
    DIP_4 = 0x0008
} DIP_Pin;

/**
 * @brief 获取拨码开关组的状态
 * @param pin 拨码开关的引脚，掩码形式
 * @return 拨码开关的状态,4位掩码
 */
INLINE uint16_t dips_get(const DIP_Pin pin)
{
    return DIP_BASE & pin;
}

/**
 * @brief 获取单个拨码开关的状态
 * @param pin 拨码开关的引脚，掩码形式
 * @return 返回的是布尔值
 */
INLINE uint16_t dip_get_single(const DIP_Pin pin)
{
    return DIP_BASE & (uint16_t)pin ? 1 : 0;
}


#ifdef __cplusplus
}
#endif
#endif //DIPS_H
