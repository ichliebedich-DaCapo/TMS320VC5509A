//
// Created by fairy on 2025/3/2 20:09.
//
#ifndef ZQ_TIMER_H
#define ZQ_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif
#include<zq_conf.h>

typedef struct
{
    volatile uint16_t CNT; // 计数器
    volatile uint16_t PRD; // 周期寄存器
    volatile uint16_t TCR; // 控制寄存器
    volatile uint16_t PRSC; // 预分频寄存器
} TIMER_Type;

#define TIMER0_BASE 0x1000
#define TIMER1_BASE 0x2400

#define TIMER0 (*(volatile ioport TIMER_Type *)TIMER0_BASE)
#define TIMER1 (*(volatile ioport TIMER_Type *)TIMER1_BASE)

/**
 * 获取定时器的计数值
 * @param timer 定时器类型
 * @return 计数值
 */
INLINE uint16_t ZQ_TIMER_Get_CNT(TIMER_Type *timer)
{
    return timer->CNT;
}

/**
 * 设置定时器的预分频
 * @param timer 定时器类型
 * @param prescaler 预分频值
 */
INLINE void ZQ_TIMER_Set_PRSC(TIMER_Type *timer, uint16_t psc)
{
    timer->PRSC = psc;
}


#ifdef __cplusplus
}
#endif
#endif //ZQ_TIMER_H
