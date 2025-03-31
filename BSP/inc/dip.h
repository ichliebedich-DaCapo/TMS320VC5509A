//
// Created by fairy on 2025/3/2 00:07.
//
#ifndef DIPS_H
#define DIPS_H


#include <zq_conf.h>

// 定义拨码开关控制寄存器地址和寄存器类型
#define DIP_BASE (*(unsigned int *)0x400002)

namespace bsp
{
    namespace dip
    {
        DECLARE_EXMEM_REGISTER_T(DIP, 0x400002);

        DECLARE_ATTRIBUTE(Pin, DIP_1 = 0x0001,
                          DIP_2 = 0x0002,
                          DIP_3 = 0x0004,
                          DIP_4 = 0x0008);
    }



    class DIP
    {
    public:
        /**
         * @brief 获取拨码开关组的状态
         * @return 拨码开关的状态,4位掩码
         */
        INLINE uint16_t get()
        {
            return dip::DIP::read();
        }

        /**
         * @brief 获取单个拨码开关的状态
         * @param pin 拨码开关的引脚，掩码形式
         * @return 返回的是布尔值
         */
        INLINE uint16_t get(const dip::Pin::Type pin)
        {
            return dip::DIP::read(pin);
        }
    };
}


#endif //DIPS_H
