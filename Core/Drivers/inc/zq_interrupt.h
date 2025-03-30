//
// Created by fairy on 2025/3/30 18:12.
//
#ifndef ZQ_INTERRUPT_H
#define ZQ_INTERRUPT_H
#include<zq_conf.h>
#include<zq_cpu.h>
namespace zq{
    namespace interrupt
    {
        namespace IFR0
        {
            typedef enum
            {
                REG = cpu::IFR0::REG,
            }Type;
        }

        struct Traits{





        };
    }
}

#endif //ZQ_INTERRUPT_H