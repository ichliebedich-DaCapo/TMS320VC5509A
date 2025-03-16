//
// Created by fairy on 2025/2/28 00:05.
//
#ifndef DEMO_ZQ_GPIO_H
#define DEMO_ZQ_GPIO_H
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
typedef enum
{
    GPIO_GROUP_NORMAL, // 普通GPIO（GPIO0-7）
    GPIO_GROUP_ADDR, // 地址总线GPIO（A0-A13）
    GPIO_GROUP_EHPI // EHPI GPIO
} GPIO_GroupType;

/* GPIO引脚*/
typedef enum
{
    GPIO_PIN_0 = 0x0000,
    GPIO_PIN_1 = 0x0001,
    GPIO_PIN_2 = 0x0002,
    GPIO_PIN_3 = 0x0003,
    GPIO_PIN_4 = 0x0004,
    GPIO_PIN_5 = 0x0005,
    GPIO_PIN_6 = 0x0006,
    GPIO_PIN_7 = 0x0007,
} GPIO_PinType_NORMAL;

typedef enum
{
    GPIO_PIN_A0 = 0x0000,
    GPIO_PIN_A1 = 0x0001,
    GPIO_PIN_A2 = 0x0002,
    GPIO_PIN_A3 = 0x0003,
    GPIO_PIN_A4 = 0x0004,
    GPIO_PIN_A5 = 0x0005,
    GPIO_PIN_A6 = 0x0006,
    GPIO_PIN_A7 = 0x0007,
    GPIO_PIN_A8 = 0x0008,
    GPIO_PIN_A9 = 0x0009,
    GPIO_PIN_A10 = 0x000A,
    GPIO_PIN_A11 = 0x000B,
    GPIO_PIN_A12 = 0x000C,
    GPIO_PIN_A13 = 0x000D,
    GPIO_PIN_A14 = 0x000E, // 仅BGA封装里有
    GPIO_PIN_A15 = 0x000F // 仅BGA封装里有
} GPIO_PinType_ADDR;

typedef enum
{
    GPIO_PIN_EHPI_8 = 0x0000,
    GPIO_PIN_EHPI_9 = 0x0001,
    GPIO_PIN_EHPI_10 = 0x0002,
    GPIO_PIN_EHPI_11 = 0x0003,
    GPIO_PIN_EHPI_12 = 0x0004,
    GPIO_PIN_EHPI_13 = 0x0005
} GPIO_PinType_EHPI;

/* GPIO方向枚举 */
typedef enum
{
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1
} GPIO_DirType;


/*
 * @brief 启用GPIO
*/
INLINE void ZQ_GPIO_Enable(GPIO_GroupType type, uint16_t pin)
{
    switch (type)
    {
        case GPIO_GROUP_ADDR:
            GPIO_AGPIOEN |= (1 << pin);
            break;
        case GPIO_GROUP_EHPI:
            GPIO_EHPIGPIOEN |= (1 << pin);
            break;
        default:
            break;
    }
}

/*
 * @brief 禁用GPIO
*/
INLINE void ZQ_GPIO_Disable(GPIO_GroupType type, uint16_t pin)
{
    switch (type)
    {
        case GPIO_GROUP_ADDR:
            GPIO_AGPIOEN &= ~(1 << pin);
            break;
        case GPIO_GROUP_EHPI:
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
INLINE void ZQ_GPIO_SetDir(GPIO_GroupType type, uint16_t pin, GPIO_DirType dir)
{
    switch (type)
    {
        case GPIO_GROUP_NORMAL:
            GPIO_IODIR = (GPIO_IODIR & ~(1 << pin)) | (dir << pin);
            break;
        case GPIO_GROUP_ADDR:
            GPIO_AGPIODIR = (GPIO_AGPIODIR & ~(1 << pin)) | (dir << pin);
            break;
        case GPIO_GROUP_EHPI:
            GPIO_EHPIGPIODIR = (GPIO_EHPIGPIODIR & ~(1 << pin)) | (dir << pin);
            break;
    }
}

/**
* @brief 获取GPIO的方向
*/
INLINE uint16_t ZQ_GPIO_GetDir(GPIO_GroupType type, uint16_t pin)
{
    switch (type)
    {
        case GPIO_GROUP_NORMAL:
            return (GPIO_IODIR >> pin) & 0x01;
        case GPIO_GROUP_ADDR:
            return (GPIO_AGPIODIR >> pin) & 0x01;
        case GPIO_GROUP_EHPI:
            return (GPIO_EHPIGPIODIR >> pin) & 0x01;
        default:
            return 0;
    }
}


/**
* @brief 设置GPIO电平
* @note 只有输出模式的情况下才能写入。这里为了省事，让程序员自己判断
*/
INLINE void ZQ_GPIO_Set(GPIO_GroupType type, uint16_t pin, uint16_t level)
{
    switch (type)
    {
        case GPIO_GROUP_NORMAL:
            GPIO_IODATA = (GPIO_IODATA & ~(1 << pin)) | (level << pin);
            break;
        case GPIO_GROUP_ADDR:
            GPIO_AGPIODATA = (GPIO_AGPIODATA & ~(1 << pin)) | (level << pin);
            break;
        case GPIO_GROUP_EHPI:
            GPIO_EHPIGPIODATA = (GPIO_EHPIGPIODATA & ~(1 << pin)) | (level << pin);
            break;
        default:
            break;
    }
}

/**
* @brief 读取GPIO电平
* @note 只有输入模式的情况下才能读取。这里为了省事，让程序员自己判断
*/
INLINE uint16_t ZQ_GPIO_Get(GPIO_GroupType type, uint16_t pin)
{
    switch (type)
    {
        case GPIO_GROUP_NORMAL:
            return (GPIO_IODATA >> pin) & 0x01;
        case GPIO_GROUP_ADDR:
            return (GPIO_AGPIODATA >> pin) & 0x01;
        case GPIO_GROUP_EHPI:
            return (GPIO_EHPIGPIODATA >> pin) & 0x01;
        default:
            return 0;
    }
}


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
        // =============== 类型安全方向枚举 ===============
        typedef enum
        {
            Dir_Input, Dir_Output
        }Dir_Type;



        // ================= 特征萃取模板 ================
        template<typename GroupTag>
        struct GPIO_Traits;

        // 模板特化
        template<>
        struct GPIO_Traits<NormalGroup>
        {
            enum
            {
                DIR_REG = 0x3400,
                DATA_REG = 0x3401,
                MAX_PIN = 7,
                NEED_ENABLE = 0
            };
        };

        template<>
        struct GPIO_Traits<AddrGroup>
        {
            enum
            {
                EN_REG = 0x4400,
                DIR_REG = 0x4404,
                DATA_REG = 0x4402,
                MAX_PIN = 15,
                NEED_ENABLE = 1
            };
        };

        template<>
        struct GPIO_Traits<EHPIGroup>
        {
            enum
            {
                EN_REG = 0x4403,
                DIR_REG = 0x4404,
                DATA_REG = 0x4405,
                MAX_PIN = 5,
                NEED_ENABLE = 1
            };
        };

        // ================= 通用GPIO引脚模板 ================
        template<typename GroupTag, uint16_t PinNum>
        class GPIO_Pin
        {
        protected:
            // 编译期特征检查
            typedef GPIO_Traits<GroupTag> Traits;

            enum
            {
                ValidPin = PinNum <= Traits::MAX_PIN
            };

            // 编译期错误触发（C++98技巧）
            struct StaticAssert
            {
                char _[ValidPin ? 1 : -1];
            };

            StaticAssert ASSERT_PIN_INVALID;


            // 编译期位掩码
            enum { PIN_MASK = 1 << PinNum };

        public:
            // 使能/禁用（仅需要使能的组）
            static void enable()
            {
                mmio::RegAccess<Traits::EN_REG>::set_bits(PIN_MASK);
            }

            static void disable()
            {
                mmio::RegAccess<Traits::EN_REG>::clear(PIN_MASK);
            }

            // 方向配置
            static void set_dir(const Dir_Type dir)
            {
                if (dir==Dir_Input)
                    mmio::RegAccess<Traits::DIR_REG>::set_bits(PIN_MASK);
                else
                    mmio::RegAccess<Traits::DIR_REG>::clear_bits(PIN_MASK);
            }

            // 电平操作
            static void high() { mmio::RegAccess<Traits::DATA_REG>::set_bits(PIN_MASK); }
            static void low() { mmio::RegAccess<Traits::DATA_REG>::clear_bits(PIN_MASK); }
            static bool read() { return (mmio::RegAccess<Traits::DATA_REG>::read() & PIN_MASK) != 0; }
        };

        // ================= 分组别名模板 ================
        template<int Pin>
        class NormalPin : public GPIO_Pin<NormalGroup, Pin>
        {
        public:
            // NormalPin() { this->set_direction(false); }
        };

        template<int Pin>
        class AddrPin : public GPIO_Pin<AddrGroup, Pin>
        {
        public:
            // AddrPin()
            // {
            //     this->enable();
            //     this->set_direction(true);
            // }
            //
            // ~AddrPin() { this->disable(); }
        };

        template<int Pin>
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
