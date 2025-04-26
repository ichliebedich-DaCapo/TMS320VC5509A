//
// Created by fairy on 2025/4/1 09:56.
//
#ifndef ZQ_ADC_H
#define ZQ_ADC_H
#include<zq_conf.h>
namespace zq
{
    namespace adc
    {
        // 声明寄存器
        DECLARE_REGISTER(CTL,0x6800);
        DECLARE_REGISTER(DATA,0x6801);
        DECLARE_REGISTER(CLKDIV,0x6802);
        DECLARE_REGISTER(CLKCTL,0x6803);

    }
}

#endif //ZQ_ADC_H