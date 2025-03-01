//
// Created by fairy on 2025/3/1 23:47.
//
#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif
#include<zq_conf.h>

// 定义指示灯寄存器地址和寄存器类型
#define LED_BASE (*((unsigned int *)0x400001))

typedef enum
{
    LED_1 = 0x0001,
    LED_2 = 0x0002,
    LED_3 = 0x0004,
    LED_4 = 0x0008,
} LED_Pin;

/**
 * @brief 点亮灯，可以把多个灯用或操作同时点亮
 * @param pin 灯的引脚号，以掩码的形式定义
 */
INLINE void led_on(const LED_Pin pin)
{
    LED_BASE |= pin;
}

/**
 * @brief 熄灭灯，可以把多个灯用或操作同时熄灭
 * @param pin 灯的引脚号，以掩码的形式定义
 */
INLINE void led_off(const LED_Pin pin)
{
    LED_BASE &= ~pin;
}

/**
 * @brief 设置灯的状态
 * @param value 灯的状态
 */
INLINE void leds_set(const uint16_t value)
{
    LED_BASE = value;
}

#ifdef __cplusplus
}
#endif
#endif //LED_H
