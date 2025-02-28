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
} ZQ_GPIO_GroupType;

/* GPIO引脚*/
typedef enum {
    GPIO_PIN_0=0x0001,
    GPIO_PIN_1=0x0002,
    GPIO_PIN_2=0x0004,
    GPIO_PIN_3=0x0008,
    GPIO_PIN_4=0x0010,
  	GPIO_PIN_5=0x0020,
    GPIO_PIN_6=0x0040,
    GPIO_PIN_7=0x0080,
} ZQ_GPIO_PinType;





#ifdef __cplusplus
}
#endif
#endif //DEMO_ZQ_GPIO_H
