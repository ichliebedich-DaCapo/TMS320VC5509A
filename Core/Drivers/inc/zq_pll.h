//
// Created by fairy on 2025/3/2 01:11.
//
#ifndef ZQ_PLL_H
#define ZQ_PLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include<zq_conf.h>

// 寄存器映射
#define SYSR (*(volatile uint16_t *)0x07FD)
#define CLKMD (*(volatile uint16_t *)0x1C00)    // 时钟模式寄存器
#define USBDPLL (*(volatile uint16_t *)0x1E00)  // 数字锁相环寄存器 为了向后兼容性，默认选择DPLL
#define USBPLLSEL (*(volatile uint16_t *)0x1E80)// USB PLL选择寄存器
#define USBAPLL (*(volatile uint16_t *)0x1F00)  // 模拟锁相环寄存器 相比数字PLL有更好的耐噪性和更少的长期抖动

// 标志
#define APLL_COUNT_MASK 0x07F8
#define APLL_COUNT_POS 0x0003
#define APLL_EN_MASK 0x0004
#define APLL_MODE_MASK 0x0002
#define APLL_STAT_MASK 0x0001

typedef enum
{
    PLL_CLKOUT_DIV_1 = 0x0000,
    PLL_CLKOUT_DIV_2 = 0x0001,
    PLL_CLKOUT_DIV_4 = 0x0002,
    PLL_CLKOUT_DIV_6 = 0x0003,
    PLL_CLKOUT_DIV_8 = 0x0004,
    PLL_CLKOUT_DIV_10 = 0x0005,
    PLL_CLKOUT_DIV_12 = 0x0006,
    PLL_CLKOUT_DIV_14 = 0x0007
} PLL_CLKOUT_DIV; // PLL输出时钟的分频系数

typedef enum
{
    PLL_DPLL = 0x0000, // 数字锁相环
    PLL_APLL = 0x0001, // 模拟锁相环
    PLL_EMPTY = 0xFFFF // 没有任何锁相环，错误！
} PLL_USBPLL_Type; // USB锁相环类型

typedef enum
{
    PLL_APLL_MULT_1 = 0x0000,
    PLL_APLL_MULT_2 = 0x0001,
    PLL_APLL_MULT_3 = 0x0002,
    PLL_APLL_MULT_4 = 0x0003,
    PLL_APLL_MULT_5 = 0x0004,
    PLL_APLL_MULT_6 = 0x0005,
    PLL_APLL_MULT_7 = 0x0006,
    PLL_APLL_MULT_8 = 0x0007,
    PLL_APLL_MULT_9 = 0x0008,
    PLL_APLL_MULT_10 = 0x0009,
    PLL_APLL_MULT_11 = 0x000A,
    PLL_APLL_MULT_12 = 0x000B,
    PLL_APLL_MULT_13 = 0x000C,
    PLL_APLL_MULT_14 = 0x000D,
    PLL_APLL_MULT_15 = 0x000E,
    PLL_APLL_MULT_16 = 0x000F
} PLL_APLL_MULT; // 锁相环倍频系数

typedef enum
{
    PLL_APLL_DIV_1 = 0x0000,
    PLL_APLL_DIV_2_OR_4 = 0x0001 // 如果倍频系数是奇数，那么就是2，如果倍频系数是偶数，那么就是4
} PLL_APLL_DIV; // 锁相环分频系数

    typedef enum
    {
        PLL_APLL_MODE_DIV = 0x0000,// 分频模式下，VCO被旁路，PLL仅作为分频器，且DIV位失效，此时PLL的分频系数由倍频决定，K为1到15，D=2，K为16，D=4
        PLL_APLL_MODE_MULT = 0x0001// 倍频模式下，VCO启用，PLL倍频和分频共同作用
    }PLL_APLL_MODE;// 锁相环工作模式

/**
 * @brief 设置PLL输出时钟的分频系数
 * @param div 输出时钟的分频系数，不能随便输入数值
 */
INLINE void ZQ_PLL_Set_CLKOUT_DIV(const PLL_CLKOUT_DIV div)
{
    SYSR = div;
}

/**
 * 获取当前锁相环类型
 * @return 锁相环枚举类型
 */
INLINE PLL_USBPLL_Type ZQ_PLL_Get_Type(void)
{
    if (USBPLLSEL & 0x0001)
        return PLL_APLL;
    if (USBPLLSEL & 0x0002)
        return PLL_DPLL;
    return PLL_EMPTY;
}

/**
 * 选择锁相环
 * @param type 锁相环类型
 */
INLINE void ZQ_PLL_Select(const PLL_USBPLL_Type type)
{
    USBPLLSEL = type;
}

/**
 * 获取APLL的计数值。USB模块在使用APLL时，计数值会从初始值开始，
 * 每16个CLKIN周期减一，为零后STAT就会被置为1，此时锁相环启用输出
 * @return APLL的计数值
 */
INLINE uint16_t ZQ_PLL_APLL_GetCount()
{
    return (USBAPLL & APLL_COUNT_MASK) >> APLL_COUNT_POS;
}

/**
 * 启用APLL
 */
INLINE void ZQ_PLL_APLL_ENABLE()
{
    USBAPLL |= APLL_EN_MASK;
}

/**
 * 禁用APLL。禁用后，即旁路模式，PLL直接输出CLKIN
 */
INLINE void ZQ_PLL_APLL_DISABLE()
{
    USBAPLL &= ~APLL_EN_MASK;
}

/**
 * 获取APLL是否启用
 * @return 启用返回1，禁用返回0
 */
INLINE uint16_t ZQ_PLL_APLL_IsEnable()
{
    return USBAPLL & APLL_EN_MASK ? 1 : 0;
}



#ifdef __cplusplus
}
#endif
#endif //ZQ_PLL_H
