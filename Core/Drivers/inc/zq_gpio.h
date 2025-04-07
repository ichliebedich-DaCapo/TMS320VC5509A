//
// Created by fairy on 2025/2/28 00:05.
//
#ifndef DEMO_ZQ_GPIO_H
#define DEMO_ZQ_GPIO_H
#include<zq_conf.h>

namespace zq
{
    namespace gpio
    {
        // ================= GPIO组类型定义 ================
        struct NormalGroup
        {
        };

        struct AddrGroup
        {
        };

        struct EHPIGroup
        {
        };

        // ================= 公共属性 ================
        // 类型安全方向枚举
        DECLARE_ATTRIBUTE(Dir,
                          Dir_Input=0,
                          Dir_Output
        );


        // ================= 特征萃取模板 ================
        template<typename GroupTag>
        struct GPIO_Traits;

        // 模板特化
        template<>
        struct GPIO_Traits<NormalGroup>
        {
            // 普通GPIO引脚
            DECLARE_REGISTER_T(DIR, 0x3400);

            DECLARE_REGISTER_T(DATA, 0x3401);

            enum
            {
                MAX_PIN = 7,
                NEED_ENABLE = 0 // 判断是否需要enable这个成员函数
            };
        };


        template<>
        struct GPIO_Traits<AddrGroup>
        {
            enum
            {
                MAX_PIN = 15,
                NEED_ENABLE = 1
            };

            // Addr GPIO引脚
            DECLARE_REGISTER_T(EN, 0x4400);

            DECLARE_REGISTER_T(DIR, 0x4404);

            DECLARE_REGISTER_T(DATA, 0x4402);
        };

        template<>
        struct GPIO_Traits<EHPIGroup>
        {
            enum
            {
                MAX_PIN = 5,
                NEED_ENABLE = 1
            };

            // EHPI引脚
            DECLARE_REGISTER_T(EN, 0x4403);

            DECLARE_REGISTER_T(DIR, 0x4404);

            DECLARE_REGISTER_T(DATA, 0x4405);
        };

        // ================= 基础模板分层设计 ===============
        template<typename GroupTag, uint16_t PinNum, bool NeedEnable>
        class GPIO_Pin_Impl;

        // 不需要Enable的组实现
        template<typename GroupTag, uint16_t PinNum>
        class GPIO_Pin_Impl<GroupTag, PinNum, false>
        {
        protected:
            typedef GPIO_Traits<GroupTag> Traits;
            // 编译期静态断言
            COMPILE_TIME_ASSERT(PinNum <= Traits::MAX_PIN, "PinNum is out of range");

        public:
            static void set_dir(const Dir::Type dir)
            {
                Traits::DIR::write(PinNum, dir);
            }

            static void high() { Traits::DATA::write(PinNum,true); }
            static void low() { Traits::DATA::clear(PinNum); }
            static bool read() { return Traits::DATA::read(PinNum); }
        };

        // 需要Enable的组实现
        template<typename GroupTag, uint16_t PinNum>
        class GPIO_Pin_Impl<GroupTag, PinNum, true>
                : public GPIO_Pin_Impl<GroupTag, PinNum, false>
        {
        protected:
            typedef GPIO_Traits<GroupTag> Traits;

        public:
            static void enable()
            {
                Traits::EN::write(PinNum,true);
            }

            static void disable()
            {
                Traits::EN::clear(PinNum);
            }
        };

        // ================= 统一接口层 ================
        template<typename GroupTag, uint16_t PinNum>
        class GPIO_Pin
                : public GPIO_Pin_Impl<GroupTag, PinNum, GPIO_Traits<GroupTag>::NEED_ENABLE>
        {
        };

        // ================= 分组别名模板 ================
        template<uint16_t Pin>
        class NormalPin : public GPIO_Pin<NormalGroup, Pin>
        {
        };

        template<uint16_t Pin>
        class AddrPin : public GPIO_Pin<AddrGroup, Pin>
        {
        };

        template<uint16_t Pin>
        class EHPIPin : public GPIO_Pin<EHPIGroup, Pin>
        {
        };

        // ================= 显示实例化 ================
        // 普通GPIO引脚
        typedef NormalPin<0> GPIO_Normal0;
        typedef NormalPin<1> GPIO_Normal1;
        typedef NormalPin<2> GPIO_Normal2;
        typedef NormalPin<3> GPIO_Normal3;
        typedef NormalPin<4> GPIO_Normal4;
        typedef NormalPin<5> GPIO_Normal5;
        typedef NormalPin<6> GPIO_Normal6;
        typedef NormalPin<7> GPIO_Normal7;
        // 地址GPIO引脚
        typedef AddrPin<0> GPIO_Addr0;
        typedef AddrPin<1> GPIO_Addr1;
        typedef AddrPin<2> GPIO_Addr2;
        typedef AddrPin<3> GPIO_Addr3;
        typedef AddrPin<4> GPIO_Addr4;
        typedef AddrPin<5> GPIO_Addr5;
        typedef AddrPin<6> GPIO_Addr6;
        typedef AddrPin<7> GPIO_Addr7;
        typedef AddrPin<8> GPIO_Addr8;
        typedef AddrPin<9> GPIO_Addr9;
        typedef AddrPin<10> GPIO_Addr10;
        typedef AddrPin<11> GPIO_Addr11;
        typedef AddrPin<12> GPIO_Addr12;
        typedef AddrPin<13> GPIO_Addr13;
        typedef AddrPin<14> GPIO_Addr14;
        typedef AddrPin<15> GPIO_Addr15;
        // EHPI引脚
        typedef EHPIPin<0> GPIO_EHPI0;
        typedef EHPIPin<1> GPIO_EHPI1;
        typedef EHPIPin<2> GPIO_EHPI2;
        typedef EHPIPin<3> GPIO_EHPI3;
        typedef EHPIPin<4> GPIO_EHPI4;
        typedef EHPIPin<5> GPIO_EHPI5;
    }
} // namespace zq::gpio


#endif //DEMO_ZQ_GPIO_H
