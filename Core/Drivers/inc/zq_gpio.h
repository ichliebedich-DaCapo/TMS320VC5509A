//
// Created by fairy on 2025/2/28 00:05.
//
#ifndef DEMO_ZQ_GPIO_H
#define DEMO_ZQ_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif
#include<zq_conf.h>

/* GPIO寄存器映射*/
#define GPIO_IODIR          (*(ioport volatile uint16_t *)0x3400)  // 方向寄存器
#define GPIO_IODATA         (*(ioport volatile uint16_t *)0x3401)  // 数据寄存器
#define GPIO_AGPIOEN        (*(ioport volatile uint16_t *)0x4400)  // 地址GPIO使能
#define GPIO_AGPIODIR       (*(ioport volatile uint16_t *)0x4404)  // 地址方向
#define GPIO_AGPIODATA      (*(ioport volatile uint16_t *)0x4402)  // 地址数据
#define GPIO_EHPIGPIOEN  	(*(ioport volatile uint16_t *)0x4403)  // EHPI GPIO使能寄存器
#define GPIO_EHPIGPIODIR 	(*(ioport volatile uint16_t *)0x4404)  // EHPI GPIO方向寄存器
#define GPIO_EHPIGPIODATA 	(*(ioport volatile uint16_t *)0x4405)  // EHPI GPIO数据寄存器

/* GPIO分组枚举 */
typedef enum {
    ZQ_GPIO_GROUP_NORMAL,   // 普通GPIO（GPIO0-7）
    ZQ_GPIO_GROUP_ADDR,     // 地址总线GPIO（A0-A13）
    ZQ_GPIO_GROUP_EHPI      // EHPI GPIO
} GPIO_GroupType;

/* GPIO引脚*/
typedef enum {
    GPIO_PIN_0=0x0000,
    GPIO_PIN_1=0x0001,
    GPIO_PIN_2=0x0002,
    GPIO_PIN_3=0x0003,
    GPIO_PIN_4=0x0004,
  	GPIO_PIN_5=0x0005,
    GPIO_PIN_6=0x0006,
    GPIO_PIN_7=0x0007,
} GPIO_PinType_NORMAL;

typedef enum {
    GPIO_PIN_A0=0x0000,
    GPIO_PIN_A1=0x0001,
    GPIO_PIN_A2=0x0002,
    GPIO_PIN_A3=0x0003,
    GPIO_PIN_A4=0x0004,
    GPIO_PIN_A5=0x0005,
    GPIO_PIN_A6=0x0006,
    GPIO_PIN_A7=0x0007,
    GPIO_PIN_A8=0x0008,
    GPIO_PIN_A9=0x0009,
    GPIO_PIN_A10=0x000A,
    GPIO_PIN_A11=0x000B,
    GPIO_PIN_A12=0x000C,
    GPIO_PIN_A13=0x000D,
    GPIO_PIN_A14=0x000E,// 仅BGA封装里有
    GPIO_PIN_A15=0x000F// 仅BGA封装里有
} GPIO_PinType_ADDR;

typedef enum {
    GPIO_PIN_EHPI_8=0x0000,
    GPIO_PIN_EHPI_9=0x0001,
    GPIO_PIN_EHPI_10=0x0002,
    GPIO_PIN_EHPI_11=0x0003,
    GPIO_PIN_EHPI_12=0x0004,
    GPIO_PIN_EHPI_13=0x0005
} GPIO_PinType_EHPI;

/* GPIO方向枚举 */
typedef enum {
    GPIO_DIR_INPUT=0,
    GPIO_DIR_OUTPUT=1
} GPIO_DirType;


/*
 * @brief 启用GPIO
*/
INLINE void ZQ_GPIO_Enable(GPIO_GroupType type,uint16_t pin)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_ADDR:
            GPIO_AGPIOEN |=(1 << pin);
            break;
        case ZQ_GPIO_GROUP_EHPI:
            GPIO_EHPIGPIOEN |=(1 << pin);
          break;
        default:
            break;
    }
}

/*
 * @brief 禁用GPIO
*/
INLINE void ZQ_GPIO_Disable(GPIO_GroupType type,uint16_t pin)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_ADDR:
            GPIO_AGPIOEN &= ~(1 << pin);
            break;
        case ZQ_GPIO_GROUP_EHPI:
            GPIO_EHPIGPIOEN &= ~(1 << pin);
            break;
        default:
            break;
    }
}


/**
* @brief 设置GPIO方向
* @param type GPIO类型
* @param pin GPIO引脚
* @param dir 方向
*/
INLINE void ZQ_GPIO_SetDir(GPIO_GroupType type,uint16_t pin,GPIO_DirType dir)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_NORMAL:
            GPIO_IODIR = (GPIO_IODIR & ~(1<<pin))| (dir << pin);
            break;
        case ZQ_GPIO_GROUP_ADDR:
            GPIO_AGPIODIR = (GPIO_AGPIODIR & ~(1<<pin))| (dir << pin);
            break;
        case ZQ_GPIO_GROUP_EHPI:
            GPIO_EHPIGPIODIR = (GPIO_EHPIGPIODIR & ~(1<<pin))| (dir << pin);
            break;
    }

}

/**
* @brief 获取GPIO的方向
*/
INLINE uint16_t ZQ_GPIO_GetDir(GPIO_GroupType type,uint16_t pin)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_NORMAL:
            return (GPIO_IODIR >> pin) & 0x01;
            break;
        case ZQ_GPIO_GROUP_ADDR:
            return (GPIO_AGPIODIR >> pin) & 0x01;
            break;
        case ZQ_GPIO_GROUP_EHPI:
            return (GPIO_EHPIGPIODIR >> pin) & 0x01;
        default:
            return 0;
    }
}


/**
* @brief 设置GPIO电平
* @note 只有输出模式的情况下才能写入。这里为了省事，让程序员自己判断
*/
INLINE void ZQ_GPIO_Set(GPIO_GroupType type,uint16_t pin,uint16_t level)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_NORMAL:
            GPIO_IODATA = (GPIO_IODATA & ~(1<<pin))| (level << pin);
            break;
        case ZQ_GPIO_GROUP_ADDR:
            GPIO_AGPIODATA = (GPIO_AGPIODATA & ~(1<<pin))| (level << pin);
            break;
        case ZQ_GPIO_GROUP_EHPI:
            GPIO_EHPIGPIODATA = (GPIO_EHPIGPIODATA & ~(1<<pin))| (level << pin);
            break;
        default:
            break;
    }
}

/**
* @brief 读取GPIO电平
* @note 只有输入模式的情况下才能读取。这里为了省事，让程序员自己判断
*/
INLINE uint16_t ZQ_GPIO_Get(GPIO_GroupType type,uint16_t pin)
{
    switch (type)
    {
        case ZQ_GPIO_GROUP_NORMAL:
            return (GPIO_IODATA >> pin) & 0x01;
            break;
        case ZQ_GPIO_GROUP_ADDR:
            return (GPIO_AGPIODATA >> pin) & 0x01;
            break;
        case ZQ_GPIO_GROUP_EHPI:
            return (GPIO_EHPIGPIODATA >> pin) & 0x01;
        default:
            return 0;
    }
}





#ifdef __cplusplus
}
#endif
#endif //DEMO_ZQ_GPIO_H
