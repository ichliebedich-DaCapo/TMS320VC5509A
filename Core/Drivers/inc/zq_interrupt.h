//
// Created by fairy on 2025/3/30 18:12.
//
#ifndef ZQ_INTERRUPT_H
#define ZQ_INTERRUPT_H
#include<zq_conf.h>
#include<zq_cpu.h>
namespace zq{
    namespace interrupt{

        struct Traits{
struct IFR
{
    enum
    {
        REG = 0x8000,
    };
};
          };

    }
}

#endif //ZQ_INTERRUPT_H