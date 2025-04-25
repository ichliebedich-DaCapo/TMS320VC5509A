//
// Created by fairy on 2025/4/26 00:43.
//
#ifndef ZQ_I2C_H
#define ZQ_I2C_H
#include <zq_conf.h>


namespace zq
{
    namespace i2c
    {
        namespace detail
        {
            DECLARE_REGISTER(OAR,  0x3C00);  // [9:0] I²C自有地址寄存器
            DECLARE_REGISTER(IER,  0x3C01);  // I²C中断使能寄存器
            DECLARE_REGISTER(STR,  0x3C02);  // I²C状态寄存器（复位值含特殊位）
            DECLARE_REGISTER(CLKL,  0x3C03);  // I²C分频寄存器低位
            DECLARE_REGISTER(CLKH, 0x3C04);  // I²C分频寄存器高位
            DECLARE_REGISTER(CNT,  0x3C05);  // I²C数据计数寄存器
            DECLARE_REGISTER(DR,   0x3C06);  // I²C数据接收寄存器[7:0] 原名为DRR
            DECLARE_REGISTER(SA,  0x3C07);  // [9:0] I²C从机地址寄存器（复位值0x03FF） 原名为SAR
            DECLARE_REGISTER(DX,   0x3C08);  // I²C数据发送寄存器[7:0] 原名为DXR
            DECLARE_REGISTER(MDR,  0x3C09);  // I²C模式寄存器[14:0]
            DECLARE_REGISTER(ISRC, 0x3C0A);  // I²C中断源寄存器
            DECLARE_REGISTER(PSC,  0x3C0C);  // I²C预分频寄存器
            DECLARE_REGISTER(MDR2, 0x3C0F);  // I²C扩展模式寄存器



        }

        class I2C
        {

        };
    }

}
#endif //ZQ_I2C_H