//
// Created by fairy on 2025/2/28 00:05.
//
#ifndef DEMO_ZQ_GPIO_H
#define DEMO_ZQ_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* GPIO寄存器映射*/
#define GPIO_IODIR          (*(ioport volatile uint16_t *)0x3400)  // 方向寄存器
#define GPIO_IODATA         (*(ioport volatile uint16_t *)0x3401)  // 数据寄存器
#define GPIO_AGPIOEN        (*(ioport volatile uint16_t *)0x4400)  // 地址GPIO使能
#define GPIO_AGPIODIR       (*(ioport volatile uint16_t *)0x4404)  // 地址方向
#define GPIO_AGPIODATA      (*(ioport volatile uint16_t *)0x4402)  // 地址数据
#define GPIO_EHPIGPIOEN     (*(ioport volatile uint16_t *)0x4403)  // EHPI/GPIO Enable Register
#define GPIO_EHPIGPIODIR    (*(ioport volatile uint16_t *)0x4404)  // EHPI/GPIO Direction Register
#define GPIO_EHPIGPIODATA   (*(ioport volatile uint16_t *)0x4405)  // EHPI/GPIO Data Register


#ifdef __cplusplus
}
#endif
#endif //DEMO_ZQ_GPIO_H
