//
// Created by fairy on 2025/3/2 01:11.
//
#ifndef ZQ_PLL_H
#define ZQ_PLL_H

#include<zq_conf.h>

// 寄存器映射
#define USBDPLL (*(volatile uint16_t *)0x1E00)  // 数字锁相环寄存器 为了向后兼容性，默认选择DPLL
#define USBPLLSEL (*(volatile uint16_t *)0x1E80)// USB PLL选择寄存器
#define USBAPLL (*(volatile uint16_t *)0x1F00)  // 模拟锁相环寄存器 相比数字PLL有更好的耐噪性和更少的长期抖动


// USBPLL_APLL
#define USBPLL_APLL_MULT_MASK 0xF000
#define USBPLL_APLL_MULT_POS 0x000C
#define USBPLL_APLL_DIV_MASK 0x0800
#define USBPLL_APLL_COUNT_MASK 0x07F8
#define USBPLL_APLL_COUNT_POS 0x0003
#define USBPLL_APLL_EN_MASK 0x0004
#define USBPLL_APLL_EN_POS 0x0003
#define USBPLL_APLL_MODE_MASK 0x0002
#define USBPLL_APLL_MODE_POS 0x0001
#define USBPLL_APLL_STAT_MASK 0x0001
// USBPLL_SEL
#define USBPLL_SEL_PLLSEL_MASK 0x0001
#define USBPLL_SEL_APLLSTAT_MASK 0x0002
#define USBPLL_SEL_APLLSTAT_POS 0x0001
#define USBPLL_SEL_DPLLSTAT_MASK 0x0004
#define USBPLL_SEL_DPLLSTAT_POS 0x0002


typedef enum
{
    USBPLL_DPLL = 0x0000, // 数字锁相环
    USBPLL_APLL = 0x0001, // 模拟锁相环
} USBPLL_Type; // USB锁相环类型


typedef enum
{
    USBPLL_APLL_MULT_1 = 0x0000,
    USBPLL_APLL_MULT_2 = 0x0001,
    USBPLL_APLL_MULT_3 = 0x0002,
    USBPLL_APLL_MULT_4 = 0x0003,
    USBPLL_APLL_MULT_5 = 0x0004,
    USBPLL_APLL_MULT_6 = 0x0005,
    USBPLL_APLL_MULT_7 = 0x0006,
    USBPLL_APLL_MULT_8 = 0x0007,
    USBPLL_APLL_MULT_9 = 0x0008,
    USBPLL_APLL_MULT_10 = 0x0009,
    USBPLL_APLL_MULT_11 = 0x000A,
    USBPLL_APLL_MULT_12 = 0x000B,
    USBPLL_APLL_MULT_13 = 0x000C,
    USBPLL_APLL_MULT_14 = 0x000D,
    USBPLL_APLL_MULT_15 = 0x000E,
    USBPLL_APLL_MULT_16 = 0x000F
} USBPLL_APLL_MULT; // 锁相环倍频系数

typedef enum
{
    USBPLL_APLL_DIV_1 = 0x0000,
    USBPLL_APLL_DIV_2_OR_4 = 0x0001 // 如果倍频系数是奇数，那么就是2，如果倍频系数是偶数，那么就是4
} USBPLL_APLL_DIV; // 锁相环分频系数

typedef enum
{
    USBPLL_APLL_MODE_DIV = 0x0000, // 分频模式下，VCO被旁路，PLL仅作为分频器，且DIV位失效，此时PLL的分频系数由倍频决定，K为1到15，D=2，K为16，D=4
    USBPLL_APLL_MODE_MULT = 0x0001 // 倍频模式下，VCO启用，PLL倍频和分频共同作用
} USBPLL_APLL_MODE; // 锁相环工作模式


typedef enum
{
    USBPLL_DPLL_MODE_NO_USB_BOOT = 0x2006, // 二分频
    USBPLL_DPLL_MODE_USB_BOOT = 0x2213 // 四倍频
} USBPLL_DPLL_MODE;


/*************************************APLL模拟器锁相环*************************************/
/**
 * @brief 设置APLL的倍频系数
 * @param mult 倍频系数，不能随便输入数值
 */
INLINE void ZQ_USBPLL_APLL_Set_MULT(const USBPLL_APLL_MULT mult)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_MULT_MASK) | (mult << USBPLL_APLL_MULT_POS);
}

/**
 * @brief 获取APLL的倍频系数
 * @return 倍频系数
 */
INLINE USBPLL_APLL_MULT ZQ_USBPLL_APLL_Get_MULT(void)
{
    return (USBPLL_APLL_MULT) ((USBAPLL & USBPLL_APLL_MULT_MASK) >> USBPLL_APLL_MULT_POS);
}

/**
 * @brief 设置APLL的分频系数
 * @param div 分频系数，不能随便输入数值
 */
INLINE void ZQ_USBPLL_APLL_Set_DIV(const USBPLL_APLL_DIV div)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_DIV_MASK) | div;
}

/**
* 获取APLL的计数值。USB模块在使用APLL时，计数值会从初始值开始，
* 每16个CLKIN周期减一，为零后STAT就会被置为1，此时锁相环启用输出
* @return APLL的计数值
*/
INLINE uint16_t ZQ_USBPLL_APLL_GetCount()
{
    return (USBAPLL & USBPLL_APLL_COUNT_MASK) >> USBPLL_APLL_COUNT_POS;
}

/**
 * 启用APLL
 */
INLINE void ZQ_USBPLL_APLL_ENABLE()
{
    USBAPLL |= USBPLL_APLL_EN_MASK;
}

/**
 * 禁用APLL。禁用后，即旁路模式，PLL直接输出CLKIN
 */
INLINE void ZQ_USBPLL_APLL_DISABLE()
{
    USBAPLL &= ~USBPLL_APLL_EN_MASK;
}

/**
 * 获取APLL是否启用
 * @return 启用返回1，禁用返回0
 */
INLINE uint16_t ZQ_USBPLL_APLL_IsEnable()
{
    return USBAPLL & USBPLL_APLL_EN_MASK ? 1 : 0;
}

/**
 * @brief 设置APLL的工作模式
 * @param mode 工作模式
 */
INLINE void ZQ_USBPLL_APLL_Set_Mode(const USBPLL_APLL_MODE mode)
{
    USBAPLL = (USBAPLL & ~USBPLL_APLL_MODE_MASK) | (mode << USBPLL_APLL_MODE_POS);
}

/**
 * 获取APLL的工作模式
 * @return 工作模式
 */
INLINE USBPLL_APLL_MODE ZQ_USBPLL_APLL_Get_Mode()
{
    return (USBPLL_APLL_MODE) ((USBAPLL & USBPLL_APLL_MODE_MASK) >> USBPLL_APLL_MODE_POS);
}


/**
 * 获取APLL状态
 * @return 状态 1为已锁定，0为未锁定
 */
INLINE uint16_t ZQ_USBPLL_APLL_Get_Stat()
{
    return USBAPLL & USBPLL_APLL_STAT_MASK;
}


/*************************************DPLL 数字锁相环*************************************/
// DPLL相关
/**
 * @brief 设置DPLL工作模式
 * @param mode 工作模式,只有二分频或者四倍频两种模式
 */
INLINE void ZQ_USBPLL_DPLL_Set_Mode(const USBPLL_DPLL_MODE mode)
{
    USBDPLL = mode;
}

/**
 * 获取DPLL工作模式
 * @return DPLL的工作模式
 */
INLINE USBPLL_DPLL_MODE ZQ_USBPLL_DPLL_Get_Mode()
{
    return (USBPLL_DPLL_MODE) USBDPLL;
}

/*
 * DPLL的设置还取决于CLKMD寄存器的设置，分频有1到4，倍频有2到31，不过手册上对这个介绍实在太少了
 */


/*************************************USB PLL 选择*************************************/
// 选择锁相环类型
/**
 * 获取当前数字锁相环锁相状态
 * @return 0：未锁定    1：已锁定
 */
INLINE uint16_t ZQ_USBPLL_Get_DPLL_FLAG(void)
{
    return (USBPLLSEL & USBPLL_SEL_APLLSTAT_MASK) >> USBPLL_SEL_APLLSTAT_POS;
}

/**
 * 获取当前模拟锁相环锁相状态
 * @return 0：未锁定    1：已锁定
*/
INLINE uint16_t ZQ_USBPLL_Get_APLL_FLAG(void)
{
    return (USBPLLSEL & USBPLL_SEL_APLLSTAT_MASK) >> USBPLL_SEL_APLLSTAT_POS;
}


/**
 * 选择锁相环
 * @param type 锁相环类型
 */
INLINE void ZQ_USBPLL_Select(const USBPLL_Type type)
{
    USBPLLSEL |= type;
}

/*************************************PLL*************************************/

// USB锁相环初始化
void ZQ_USBPLL_Init();

// PLL初始化
void ZQ_PLL_Init();


namespace zq
{
    namespace pll
    {
        // =========================== 基础属性声明 ===========================
        // PLL类型标签
        struct MainPLLTag
        {
        };

        struct USB_APLLTag
        {
        };

        struct USB_DPLLTag
        {
        };

        // 通用特征模板
        template<typename PLLType>
        struct PLL_Traits;

        // 主PLL特征
        template<>
        struct PLL_Traits<MainPLLTag>
        {
            // ====================== SYSR寄存器 ======================
            struct SYSR
            {
                /// @brief 系统寄存器地址（SYSR）
                /// @note 控制系统时钟输出分频系数
                enum
                {
                    REG = 0x07FD
                };

                struct DIV
                {
                    /// @brief 系统时钟分频配置
                    enum
                    {
                        /// @brief CLKOUT分频位掩码（3位）
                        /// @note 控制SYSCLK输出分频系数，值范围0-7
                        MASK = 0x0007,

                        /// @brief CLKOUT分频位偏移量（位0）
                        OFFSET = 0x0000
                    };
                };
            };

            // ====================== CLKMD寄存器 ======================
            struct CLKMD
            {
                /// @brief 时钟模式寄存器地址（CLKMD）
                /// @note 控制PLL倍频、分频、使能等核心功能
                enum
                {
                    REG = 0x1C00
                };

                /// @brief IAI位配置（位14）
                /// @note 退出Idle状态后的PLL锁定行为控制
                struct IAI
                {
                    enum
                    {
                        MASK = 0x4000, ///< 位掩码（bit14）
                        OFFSET = 0x000E ///< 位偏移量（14-14）
                    };
                };

                /// @brief IOB位配置（位13）
                /// @note PLL失锁应急处置控制
                struct IOB
                {
                    enum
                    {
                        MASK = 0x2000, ///< 位掩码（bit13）
                        OFFSET = 0x000D, ///< 位偏移量（13-13）
                    };
                };

                /// @brief TEST位配置（位12）
                /// @warning 必须保持为0
                struct TEST
                {
                    enum
                    {
                        MASK = 0x1000, ///< 位掩码（bit12）
                        OFFSET = 0x000C, ///< 位偏移量（12-12）
                    };
                };

                /// @brief 倍频系数配置域（位7-11）
                /// @note 5位宽，有效值范围2-31（实际值=配置值+1）
                struct MULT
                {
                    enum
                    {
                        MASK = 0x0F80, ///< 位掩码（bit7-11）
                        OFFSET = 7, ///< 位偏移量（7-11）
                        WIDTH = 5, ///< 位域宽度
                        MIN = 2, ///< 最小倍频系数（对应寄存器值0x0002）
                        MAX = 31 ///< 最大倍频系数（对应寄存器值0x001F）
                    };
                };

                /// @brief 分频系数配置域（位5-6）
                /// @note 2位宽，有效值范围0-3（实际分频=配置值+1）
                struct DIV
                {
                    enum
                    {
                        MASK = 0x0060, ///< 位掩码（bit5-6）
                        OFFSET = 5, ///< 位偏移量（5-6）
                        WIDTH = 2, ///< 位域宽度
                        MIN = 0, ///< 最小分频配置值（1分频）
                        MAX = 3 ///< 最大分频配置值（4分频）
                    };
                };

                /// @brief PLL使能控制位（位4）
                struct ENABLE
                {
                    enum
                    {
                        MASK = 0x0008, ///< 位掩码（bit4）
                        OFFSET = 4, ///< 位偏移量（4-4）
                    };
                };

                /// @brief 旁路模式分频配置（位2-3）
                /// @note 仅在旁路模式（BYPASS）下生效
                struct BYPASS_DIV
                {
                    enum
                    {
                        MASK = 0x000C, ///< 位掩码（bit2-3）
                        OFFSET = 2, ///< 位偏移量（2-3）
                        WIDTH = 2 ///< 位域宽度
                    };
                };

                /// @brief 失锁状态标志位（位1）
                /// @note 只读位，指示PLL当前锁定状态
                struct BREAKLN
                {
                    enum
                    {
                        MASK = 0x0002, ///< 位掩码（bit1）
                        OFFSET = 1, ///< 位偏移量（1-1）
                    };
                };

                /// @brief 工作模式标志位（位0）
                /// @note 只读位，指示当前工作模式
                struct LOCK
                {
                    enum
                    {
                        MASK = 0x0001, ///< 位掩码（bit0）
                        OFFSET = 0, ///< 位偏移量（0-0）
                    };
                };
            };
        };


        // USB模拟PLL特征
        template<>
        struct PLL_Traits<USB_APLLTag>
        {
            enum
            {
                USBAPLL_REG = 0x1F00,
                MULT_OFFSET = 12,
                MULT_MASK = 0xF000,
                // 其他特征...
            };
        };

        // USB数字PLL特征
        template<>
        struct PLL_Traits<USB_DPLLTag>
        {
            enum
            {
                USBDPLL_REG = 0x1E00,
                MODE_MASK = 0xFFFF,
                // 其他特征...
            };
        };

        // ========================= 枚举声明 =========================

        // 主锁相环
        struct Main
        {
            // 分频系数
            struct DIV
            {
                typedef enum
                {
                    DIV_1 = 0x0000,
                    DIV_2 = 0x0001,
                    DIV_3 = 0x0002,
                    DIV_4 = 0x0003
                } Type;
            };

            // 输出时钟的分频系数
            struct CLKOUT_DIV
            {
                typedef enum
                {
                    DIV_1 = 0x0000,
                    DIV_2 = 0x0001,
                    DIV_4 = 0x0002,
                    DIV_6 = 0x0003,
                    DIV_8 = 0x0004,
                    DIV_10 = 0x0005,
                    DIV_12 = 0x0006,
                    DIV_14 = 0x0007
                } Type;
            };

            /// @brief PLL工作模式
            struct Mode
            {
                // IAI：退出Idle状态后的PLL锁定行为控制
                struct IAI
                {
                    typedef enum
                    {
                        SAME = 0x0000, // 保持当前工作模式
                        RELOCK = 0x0001 // 重新锁定PLL
                    } Type;
                };

                // IOB：失锁应急处置控制
                struct IOB
                {
                    typedef enum
                    {
                        NO_INTERRUPT = 0x0000, ///< 不中断PLL继续输出
                        SWITCH_BYPASS = 0x0001 ///< 切换旁路模式并重启
                    } Type;
                };

                // 旁路分频系数
                struct BYPASS_DIV
                {
                    typedef enum
                    {
                        DIV_1 = 0x0000,
                        DIV_2 = 0x0001,
                        DIV_4 = 0x0002
                    } Type;
                };
            };

            // 锁相环状态
            // 不过由于这些位都比较简单，所以这个只是摆设
            struct Status
            {
                // 失锁状态标志位
                struct BREAKLN
                {
                    typedef enum
                    {
                        UNLOCK = 0x0000, ///< PLL已失锁
                        LOCKED = 0x0001 ///< PLL处于锁定状态
                    } Type;
                };
            };
        };

        // ========================= 属性控制 =========================
        // 类型安全倍频系数
        template<uint16_t Min, uint16_t Max>
        class PLL_Multiplier
        {
        public:
            template<uint16_t Val>
            static PLL_Multiplier create()
            {
                COMPILE_TIME_ASSERT(Val >= Min && Val <= Max, "PLL multiplier value out of range");
                return Val;
            }
        };

        typedef PLL_Multiplier<2, 31> MainPLL_Mult; // 主PLL倍频范围
        typedef PLL_Multiplier<1, 16> USBAPLL_Mult; // USB APLL倍频范围

        // ========================= 锁相环控制 =========================
        template<typename PLLType>
        class PLL_Controller;

        // 主PLL特化
        template<>
        class PLL_Controller<MainPLLTag>
        {
            typedef PLL_Traits<MainPLLTag>::SYSR SYSR;
            typedef PLL_Traits<MainPLLTag>::CLKMD CLKMD;
            typedef mmio::RegAccess<SYSR::REG> SYSR_REG; // 系统寄存器
            typedef mmio::RegAccess<CLKMD::REG> CLKMD_REG; // 时钟模式控制寄存器

        public:
            /// @brief 使能PLL，进入PLL锁定模式
            static void enable()
            {
                CLKMD_REG::set_bit(CLKMD::ENABLE::MASK);
            }

            /// @brief 禁用PLL，进入旁路模式
            static void disable()
            {
                CLKMD_REG::clear_bit(CLKMD::ENABLE::MASK);
            }

            // 设置输出分频系数
            static void set_out_divider(const Main::CLKOUT_DIV::Type div)
            {
                SYSR_REG::modify_bits(div, SYSR::DIV::MASK, SYSR::DIV::OFFSET);
            }

            // 设置分频系数
            static void set_divider(const Main::DIV::Type div)
            {
                SYSR_REG::modify_bits(div, CLKMD::DIV::MASK, CLKMD::DIV::OFFSET);
            }

            // 设置倍频系数
            template<typename MultType>
            static void set_multiplier(MultType mult)
            {
                CLKMD_REG::modify_bits(mult, CLKMD::MULT::MASK, CLKMD::MULT::OFFSET);
            }

            // 检查是否处于失锁状态:true表示已经失锁
            static bool is_broken()
            {
                return CLKMD_REG::read_bit_not(CLKMD::BREAKLN::MASK);
            }

            // 检查PLL是否锁定
            static bool is_locked()
            {
                return CLKMD_REG::read_bit(CLKMD::LOCK::MASK);
            }

            /// 设置PLL的工作模式
            INLINE void set_mode(const Main::Mode::IAI::Type iai, // 退出Idle后的PLL锁定行为
                                 const Main::Mode::IOB::Type iob, // 失锁处理方式
                                 const Main::Mode::BYPASS_DIV::Type bypass_div // 旁路模式下的分频系数
            )
            {
                CLKMD_REG::clear_bit(CLKMD::TEST::MASK); // 测试位必须置为0
                CLKMD_REG::modify_bits(iai, CLKMD::IAI::MASK, CLKMD::IAI::OFFSET);
                CLKMD_REG::modify_bits(iob, CLKMD::IOB::MASK, CLKMD::IOB::OFFSET);
                CLKMD_REG::modify_bits(bypass_div, CLKMD::BYPASS_DIV::MASK, CLKMD::BYPASS_DIV::OFFSET);
            }
        };

        // USB APLL特化
        template<>
        class PLL_Controller<USB_APLLTag>
        {
            typedef PLL_Traits<USB_APLLTag> Traits;

        public:
            template<typename MultType>
            static void set_multiplier(MultType mult)
            {
            }

            static void enable()
            {
                mmio::RegAccess<Traits::USBAPLL_REG>::set_bit(0x0004);
            }

            static bool is_ready()
            {
                return (mmio::RegAccess<Traits::USBAPLL_REG>::read() & 0x0001) != 0;
            }
        };

        // ========================= 具体锁相环 =========================
        // 主PLL配置器
        class MainPLL
        {
        public:
            /**
             * @brief 设置数字 PLL 锁相环时钟频率
             * @note 为了简化配置，默认启用APLL，并且已经配置为最高可用频率。此处CVDD接1.6V，因此最高推荐频率为200MHz，
             *       而外部晶振为20MHz，因此倍频为10
             * @details 默认配置模式：
             *              退出Idle后的PLL锁定行为：保持退出前的锁定状态
             *              失锁处理方式：切换到旁路模式
             *              旁路模式下的分频系数：不分频
             */
            template<uint16_t Mult>
            static void configure(const Main::DIV::Type div = Main::DIV::DIV_1,
                                  const Main::CLKOUT_DIV::Type out_div = Main::CLKOUT_DIV::DIV_4,
                                  const Main::Mode::IAI::Type iai = Main::Mode::IAI::SAME, // 退出Idle后的PLL锁定行为
                                  const Main::Mode::IOB::Type iob = Main::Mode::IOB::SWITCH_BYPASS, // 失锁处理方式
                                  const Main::Mode::BYPASS_DIV::Type bypass_div = Main::Mode::BYPASS_DIV::DIV_1
                                  // 旁路模式下的分频系数
            )
            {
                Controller::disable();
                while (Controller::is_locked()); // 等待失锁

                // 等待解锁
                Controller::set_divider(div); // 不分频
                Controller::set_out_divider(out_div); // 输出不分频
                // 设置PLL工作模式——
                Controller::set_mode(iai, iob, bypass_div);
                Controller::set_multiplier(MainPLL_Mult::create<Mult>()); // 设置倍频系数
                Controller::enable();

                while (!Controller::is_locked()); // 等待锁定
            }

        private:
            typedef PLL_Controller<MainPLLTag> Controller;
        };

        // USB APLL配置器
        class USB_APLL
        {
        public:
            template<uint16_t Mult>
            static void configure()
            {
                Controller::set_multiplier(USBAPLL_Mult::create<Mult>());
                Controller::enable();

                while (!Controller::is_ready()); // 等待就绪
            }

        private:
            typedef PLL_Controller<USB_APLLTag> Controller;
        };
    } // namespace pll
} // namespace zq

#endif //ZQ_PLL_H
