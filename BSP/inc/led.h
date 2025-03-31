//
// Created by fairy on 2025/3/1 23:47.
//
#ifndef LED_H
#define LED_H


#include<zq_conf.h>

// 定义指示灯寄存器地址和寄存器类型
#define LED_BASE (*((volatile unsigned int *)0x400001))

namespace bsp
{
    namespace led
    {
        DECLARE_REGISTER(LED, 0x400001);

        DECLARE_ATTRIBUTE(pin,
                          LED_1 = 0x0001,
                          LED_2 = 0x0002,
                          LED_3 = 0x0004,
                          LED_4 = 0x0008
        );
    }

    class LED
    {
        typedef zq::mmio::ExMemAccess<led::LED::REG> LED_Reg;

    public:
        /**
         * @brief 点亮灯，可以把多个灯用或操作同时点亮
         * @param pin 灯的引脚号，以掩码的形式定义。可以使用或操作
         */
        INLINE void on(const led::pin::Type pin)
        {
            LED_Reg::set_bit(pin);
        }

        /**
         * @brief 熄灭灯，可以把多个灯用或操作同时熄灭
         * @param pin 灯的引脚号，以掩码的形式定义。可以使用或操作
         */
        INLINE void off(const led::pin::Type pin)
        {
            LED_Reg::clear_bit(pin);
        }

        INLINE void set(const uint16_t value)
        {
            LED_Reg::write(value);
        }
    };
}


#endif //LED_H
