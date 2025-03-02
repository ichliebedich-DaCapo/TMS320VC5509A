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
#define SYSR (*(volatile uint16_t *)0x07FD)     // 设置输出时钟的分频系数
#define CLKMD (*(volatile uint16_t *)0x1C00)    // 时钟模式寄存器
#define USBDPLL (*(volatile uint16_t *)0x1E00)  // 数字锁相环寄存器 为了向后兼容性，默认选择DPLL
#define USBPLLSEL (*(volatile uint16_t *)0x1E80)// USB PLL选择寄存器
#define USBAPLL (*(volatile uint16_t *)0x1F00)  // 模拟锁相环寄存器 相比数字PLL有更好的耐噪性和更少的长期抖动

// SYSR
#define PLL_SYSR_CLKOUT_DIV_MASK 0x0007

// CLKMD
#define PLL_IAI_MASK 0x4000 // 退出Idle状态后，决定PLL是否重新锁定。0：将使用进入Idle前的设置来锁定 1：重新锁定
#define PLL_IAI_POS 0x000E
#define PLL_IOB_MASK 0x2000 // 失锁应急控制位。0：不中断PLL，继续输出 1：切换为旁路模式，重新锁相
#define PLL_IOB_POS 0x000D
#define PLL_TEST_MASK 0x1000 // 必须保持为0
#define PLL_TEST_POS 0x000C
#define PLL_MULT_MASK 0x0F80 // 倍频系数，值在2到31
#define PLL_MULT_POS 0x0007
#define PLL_DIV_MASK 0x0060 // 分频系数，值在0到3
#define PLL_DIV_POS 0x0005
#define PLL_ENABLE_MASK 0x0008 // 使能位。0：旁路模式 1：启用PLL输出，进入锁定模式
#define PLL_ENABLE_POS 0x0004
#define PLL_BYPASS_DIV_MASK 0x000C // 旁路模式下的分频。00：一分频 01：二分频 10或11：四分频
#define PLL_BYPASS_DIV_POS 0x0002
#define PLL_BREAKLN_MASK  0x0002// PLL失锁标志 0：PLL已经失锁 1：锁定状态或对CLKMD寄存器的写操作
#define PLL_BREAKLN_POS 0x0001
#define PLL_LOCK_MASK 0x0001    // PLL锁定模式标志。0：处于旁路模式 1：锁定模式
#define PLL_LOCK_POS 0x0000

// USBPLL_APLL
#define USBPLL_APLL_MULT_MASK 0xF000
#define USBPLL_APLL_MULT_POS 0x000C
#define USBPLL_APLL_DIV_MASK 0x0800
#define USBPLL_APLL_COUNT_MASK 0x07F8
#define USBPLL_APLL_COUNT_POS 0x0003
#define USBPLL_APLL_EN_MASK 0x0004
#define USBPLL_APLL_EN_POS 0x0003
#define USBPLL_APLL_MODE_MASK 0x0002
#define USBPLL_APLL_MODE_POS 0x0001
#define USBPLL_APLL_STAT_MASK 0x0001
// USBPLL_SEL
#define USBPLL_SEL_PLLSEL_MASK 0x0001
#define USBPLL_SEL_APLLSTAT_MASK 0x0002
#define USBPLL_SEL_APLLSTAT_POS 0x0001
#define USBPLL_SEL_DPLLSTAT_MASK 0x0004
#define USBPLL_SEL_DPLLSTAT_POS 0x0002


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

// IAI位 (位14) - 退出Idle后的PLL锁定行为
typedef enum
{
    PLL_IAI_SAME = 0x0000, // 使用进入Idle前的设置
    PLL_IAI_RELOCK = 0x0001 // 重新锁定PLL
} PLL_IAI;

// IOB位 (位13) - 失锁应急处置
typedef enum
{
    PLL_IOB_NO_INTERRUPT = 0x0000, // 不中断PLL，继续输出
    PLL_IOB_SWITCH_BYPASS = 0x0001 // 切换为旁路模式，并重启
} PLL_IOB;

// PLL_ENABLE位 (位4) - PLL使能控制
typedef enum
{
    PLL_ENABLE_BYPASS = 0x0000, // 旁路模式，即关闭锁相环
    PLL_ENABLE_LOCK = 0x0001 // 锁定模式
} PLL_ENABLE;

// BYPASS DIV位（位2~3）- 旁路模式下的分频
typedef enum
{
    PLL_BYPASS_DIV_1 = 0x0000, // 1分频
    PLL_BYPASS_DIV_2 = 0x0001, // 2分频
    PLL_BYPASS_DIV_4 = 0x0002, // 4分频 实际值为2或者3，这里使用2
} PLL_BYPASS_DIV;


// BREAKLN位 (位1) - 失锁状态标志
typedef enum
{
    PLL_BREAKLN_UNLOCK = 0x0000, // PLL已失锁
    PLL_BREAKLN_LOCK = 0x0001 // PLL已经锁定
} PLL_BREAKLN;

// LOCK位 (位0) - 工作模式标志
typedef enum
{
    PLL_LOCK_FLAG_BYPASS = 0x0000, // 处于旁路模式
    PLL_LOCK_FLAG_LOCK = 0x0001 // 处于锁定模式
} PLL_LOCK_FLAG;

typedef enum
{
    PLL_DIV_1 = 0x0000,
    PLL_DIV_2 = 0x0001,
    PLL_DIV_3 = 0x0002,
    PLL_DIV_4 = 0x0003
} PLL_DIV; // PLL的分频系数

typedef enum
{
    PLL_MULT_2 = 0x0002,
    PLL_MULT_3 = 0x0003,
    PLL_MULT_4 = 0x0004,
    PLL_MULT_5 = 0x0005,
    PLL_MULT_6 = 0x0006,
    PLL_MULT_7 = 0x0007,
    PLL_MULT_8 = 0x0008,
    PLL_MULT_9 = 0x0009,
    PLL_MULT_10 = 0x000A,
    PLL_MULT_11 = 0x000B,
    PLL_MULT_12 = 0x000C,
    PLL_MULT_13 = 0x000D,
    PLL_MULT_14 = 0x000E,
    PLL_MULT_15 = 0x000F,
    PLL_MULT_16 = 0x0010,
    PLL_MULT_17 = 0x0011,
    PLL_MULT_18 = 0x0012,
    PLL_MULT_19 = 0x0013,
    PLL_MULT_20 = 0x0014,
    PLL_MULT_21 = 0x0015,
    PLL_MULT_22 = 0x0016,
    PLL_MULT_23 = 0x0017,
    PLL_MULT_24 = 0x0018,
    PLL_MULT_25 = 0x0019,
    PLL_MULT_26 = 0x001A,
    PLL_MULT_27 = 0x001B,
    PLL_MULT_28 = 0x001C,
    PLL_MULT_29 = 0x001D,
    PLL_MULT_30 = 0x001E,
    PLL_MULT_31 = 0x001F
} PLL_MULT; // PLL的倍频系数


typedef enum
{
    USBPLL_DPLL = 0x0000, // 数字锁相环
    USBPLL_APLL = 0x0001, // 模拟锁相环
} USBPLL_Type; // USB锁相环类型


typedef enum
{
    USBPLL_APLL_MULT_1 = 0x0000,
    USBPLL_APLL_MULT_2 = 0x0001,
    USBPLL_APLL_MULT_3 = 0x0002,
    USBPLL_APLL_MULT_4 = 0x0003,
    USBPLL_APLL_MULT_5 = 0x0004,
    USBPLL_APLL_MULT_6 = 0x0005,
    USBPLL_APLL_MULT_7 = 0x0006,
    USBPLL_APLL_MULT_8 = 0x0007,
    USBPLL_APLL_MULT_9 = 0x0008,
    USBPLL_APLL_MULT_10 = 0x0009,
    USBPLL_APLL_MULT_11 = 0x000A,
    USBPLL_APLL_MULT_12 = 0x000B,
    USBPLL_APLL_MULT_13 = 0x000C,
    USBPLL_APLL_MULT_14 = 0x000D,
    USBPLL_APLL_MULT_15 = 0x000E,
    USBPLL_APLL_MULT_16 = 0x000F
} USBPLL_APLL_MULT; // 锁相环倍频系数

typedef enum
{
    USBPLL_APLL_DIV_1 = 0x0000,
    USBPLL_APLL_DIV_2_OR_4 = 0x0001 // 如果倍频系数是奇数，那么就是2，如果倍频系数是偶数，那么就是4
} USBPLL_APLL_DIV; // 锁相环分频系数

typedef enum
{
    USBPLL_APLL_MODE_DIV = 0x0000, // 分频模式下，VCO被旁路，PLL仅作为分频器，且DIV位失效，此时PLL的分频系数由倍频决定，K为1到15，D=2，K为16，D=4
    USBPLL_APLL_MODE_MULT = 0x0001 // 倍频模式下，VCO启用，PLL倍频和分频共同作用
} USBPLL_APLL_MODE; // 锁相环工作模式


typedef enum
{
    USBPLL_DPLL_MODE_NO_USB_BOOT = 0x2006, // 二分频
    USBPLL_DPLL_MODE_USB_BOOT = 0x2213 // 四倍频
} USBPLL_DPLL_MODE;

/*************************************PLL*************************************/
// =========锁相环时钟输出=========
/**
 * @brief 设置PLL输出时钟的分频系数
 * @param div 输出时钟的分频系数，不能随便输入数值
 */
INLINE void ZQ_PLL_Set_CLKOUT_DIV(const PLL_CLKOUT_DIV div)
{
    SYSR = div;
}

/**
 * @brief 获取PLL输出时钟的分频系数
 * @return 输出时钟的分频系数
 */
INLINE PLL_CLKOUT_DIV ZQ_PLL_Get_CLKOUT_DIV(void)
{
    return SYSR & PLL_SYSR_CLKOUT_DIV_MASK;
}

// =========锁相环时钟生成器=========
/**
 * 设置PLL退出Idle后的PLL锁定行为
 * @param iai IAI模式
 */
INLINE void ZQ_PLL_Set_IAI(const PLL_IAI iai)
{
    CLKMD = (CLKMD & ~PLL_IAI_MASK) | (iai << PLL_IAI_POS);
}

/**
 * 设置PLL失锁时处理方式
 * @param iob 处理失锁的方式
 */
INLINE void ZQ_PLL_Set_IOB(const PLL_IOB iob)
{
    CLKMD = (CLKMD & ~PLL_IOB_MASK) | (iob << PLL_IOB_POS);
}

/**
 * Test位必须置为0
 */
INLINE void ZQ_PLL_Set_TEST()
{
    CLKMD &= ~PLL_TEST_MASK;
}

/**
 * 设置PLL的倍频系数
 * @param mult 倍频系数，在2到31之间
 */
INLINE void ZQ_PLL_Set_MULT(const PLL_MULT mult)
{
    CLKMD = (CLKMD & ~PLL_MULT_MASK) | (mult << PLL_MULT_POS);
}

/**
 * 设置分频系数
 * @param div 分频系数，在1到4之间（寄存器的值实际在0到3）
 */
INLINE void ZQ_PLL_Set_DIV(const PLL_DIV div)
{
    CLKMD = (CLKMD & ~PLL_DIV_MASK) | (div << PLL_DIV_POS);
}


/**
 * @brief 使能PLL，进入PLL锁定模式
 */
INLINE void ZQ_PLL_ENABLE()
{
    CLKMD |= PLL_ENABLE_MASK;
}

/**
 * @brief 禁用PLL，进入旁路模式
 */
INLINE void ZQ_PLL_DISABLE()
{
    CLKMD &= ~PLL_ENABLE_MASK;
}

/**
 * 设置旁路模式下的分频系数
 * @param div 分频系数
 */
INLINE void ZQ_PLL_Set_BYPASS_DIV(const PLL_BYPASS_DIV div)
{
    CLKMD = (CLKMD & ~PLL_BYPASS_DIV_MASK) | (div << PLL_BYPASS_DIV_POS);
}

/**
 * @brief 检查PLL的失锁状态标志
 * @return 失锁状态标志
 */
INLINE PLL_BREAKLN ZQ_PLL_Get_BREAKLN(void)
{
    return (CLKMD & PLL_BREAKLN_MASK) >> PLL_BREAKLN_POS;
}

/**
 * @brief 检查PLL的工作模式
 * @return 工作模式
 */
INLINE PLL_LOCK_FLAG ZQ_PLL_Get_LOCK_FLAG(void)
{
    return (CLKMD & PLL_LOCK_MASK) >> PLL_LOCK_POS;
}


/*************************************APLL模拟器锁相环*************************************/
// APLL相关
/**
 * @brief 设置APLL的倍频系数
 * @param mult 倍频系数，不能随便输入数值
 */
INLINE void ZQ_USBPLL_APLL_Set_MULT(const USBPLL_APLL_MULT mult)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_MULT_MASK) | (mult << USBPLL_APLL_MULT_POS);
}

/**
 * @brief 获取APLL的倍频系数
 * @return 倍频系数
 */
INLINE USBPLL_APLL_MULT ZQ_USBPLL_APLL_Get_MULT(void)
{
    return (USBAPLL & USBPLL_APLL_MULT_MASK) >> USBPLL_APLL_MULT_POS;
}

/**
 * @brief 设置APLL的分频系数
 * @param div 分频系数，不能随便输入数值
 */
INLINE void ZQ_USBPLL_APLL_Set_DIV(const USBPLL_APLL_DIV div)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_DIV_MASK) | div;
}

/**
* 获取APLL的计数值。USB模块在使用APLL时，计数值会从初始值开始，
* 每16个CLKIN周期减一，为零后STAT就会被置为1，此时锁相环启用输出
* @return APLL的计数值
*/
INLINE uint16_t ZQ_USBPLL_APLL_GetCount()
{
    return (USBAPLL & USBPLL_APLL_COUNT_MASK) >> USBPLL_APLL_COUNT_POS;
}

/**
 * 启用APLL
 */
INLINE void ZQ_USBPLL_APLL_ENABLE()
{
    USBAPLL |= USBPLL_APLL_EN_MASK;
}

/**
 * 禁用APLL。禁用后，即旁路模式，PLL直接输出CLKIN
 */
INLINE void ZQ_USBPLL_APLL_DISABLE()
{
    USBAPLL &= ~USBPLL_APLL_EN_MASK;
}

/**
 * 获取APLL是否启用
 * @return 启用返回1，禁用返回0
 */
INLINE uint16_t ZQ_USBPLL_APLL_IsEnable()
{
    return USBAPLL & USBPLL_APLL_EN_MASK ? 1 : 0;
}

/**
 * @brief 设置APLL的工作模式
 * @param mode 工作模式
 */
INLINE void ZQ_USBPLL_APLL_Set_Mode(const USBPLL_APLL_MODE mode)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_MODE_MASK) | (mode << USBPLL_APLL_MODE_POS);
}

/**
 * 获取APLL的工作模式
 * @return 工作模式
 */
INLINE USBPLL_APLL_MODE ZQ_USBPLL_APLL_Get_Mode()
{
    return (USBAPLL & USBPLL_APLL_MODE_MASK) >> USBPLL_APLL_MODE_POS;
}


/**
 * 获取APLL状态
 * @return 状态 1为已锁定，0为未锁定
 */
INLINE uint16_t ZQ_USBPLL_APLL_Get_Stat()
{
    return USBAPLL & USBPLL_APLL_STAT_MASK;
}


/*************************************DPLL 数字锁相环*************************************/
// DPLL相关
/**
 * @brief 设置DPLL工作模式
 * @param mode 工作模式,只有二分频或者四倍频两种模式
 */
INLINE void ZQ_USBPLL_DPLL_Set_Mode(const USBPLL_DPLL_MODE mode)
{
    USBDPLL = mode;
}

/**
 * 获取DPLL工作模式
 * @return DPLL的工作模式
 */
INLINE USBPLL_DPLL_MODE ZQ_USBPLL_DPLL_Get_Mode()
{
    return USBDPLL;
}

/*
 * DPLL的设置还取决于CLKMD寄存器的设置，分频有1到4，倍频有2到31，不过手册上对这个介绍实在太少了
 */


/*************************************USB PLL 选择*************************************/
// 选择锁相环类型
/**
 * 获取当前数字锁相环锁相状态
 * @return 0：未锁定    1：已锁定
 */
INLINE uint16_t ZQ_USBPLL_Get_DPLL_FLAG(void)
{
    return (USBPLLSEL & USBPLL_SEL_APLLSTAT_MASK) >> USBPLL_SEL_APLLSTAT_POS;
}

/**
 * 获取当前模拟锁相环锁相状态
 * @return 0：未锁定    1：已锁定
*/
INLINE uint16_t ZQ_USBPLL_Get_APLL_FLAG(void)
{
    return (USBPLLSEL & USBPLL_SEL_APLLSTAT_MASK) >> USBPLL_SEL_APLLSTAT_POS;
}


/**
 * 选择锁相环
 * @param type 锁相环类型
 */
INLINE void ZQ_USBPLL_Select(const USBPLL_Type type)
{
    USBPLLSEL |= type;
}

/*************************************PLL*************************************/

// USB锁相环初始化
void ZQ_USBPLL_Init();

// PLL初始化
void ZQ_PLL_Init();


#ifdef __cplusplus
}
#endif
#endif //ZQ_PLL_H
