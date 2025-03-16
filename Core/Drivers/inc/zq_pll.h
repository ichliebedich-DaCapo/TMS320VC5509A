//
// Created by fairy on 2025/3/2 01:11.
//
#ifndef ZQ_PLL_H
#define ZQ_PLL_H
#include<zq_conf.h>

namespace zq
{
    namespace pll
    {
        // =========================== 基础属性声明 ===========================
        // PLL类型标签
        struct MainPLLTag
        {
        };

        struct USB_PLLTag
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
        struct PLL_Traits<USB_PLLTag>
        {
            // 数字锁相环寄存器 为了向后兼容性，默认选择DPLL
            struct DPLL
            {
                enum
                {
                    REG = 0x1E00
                };
            };

            // 模拟锁相环寄存器 相比数字PLL有更好的耐噪性和更少的长期抖动
            struct APLL
            {
                enum
                {
                    REG = 0x1F00
                };

                // 倍频系数:bit12-15
                struct MULT
                {
                    enum
                    {
                        MASK = 0xF000,
                        OFFSET = 12
                    };
                };

                // 分频系数:bit11
                struct DIV
                {
                    enum
                    {
                        MASK = 0x0800,
                        OFFSET = 11
                    };
                };

                // 计数：bit3-11
                struct COUNT
                {
                    enum
                    {
                        MASK = 0x07F8,
                        OFFSET = 3
                    };
                };

                // 使能位：bit2
                struct ENABLE
                {
                    enum
                    {
                        MASK = 0x0004,
                        OFFSET = 2
                    };
                };

                // 模式位：bit1
                struct MODE
                {
                    enum
                    {
                        MASK = 0x0002,
                        OFFSET = 1
                    };
                };

                // 状态位：bit0
                struct STAT
                {
                    enum
                    {
                        MASK = 0x0001,
                        OFFSET = 0
                    };
                };
            };

            // USB PLL选择寄存器
            struct SEL
            {
                enum
                {
                    REG = 0x1E80
                };

                // 选择位：bit0
                struct PLLSEL
                {
                    enum
                    {
                        MASK = 0x0001,
                        OFFSET = 0
                    };
                };

                // APLL状态位：bit1
                struct APLLSTAT
                {
                    enum
                    {
                        MASK = 0x0002,
                        OFFSET = 1
                    };
                };

                // DPLL状态位：bit2
                struct DPLLSTAT
                {
                    enum
                    {
                        MASK = 0x0004,
                        OFFSET = 2
                    };
                };
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

        struct USB
        {
            // USB锁相环类型
            struct SEL
            {
                typedef enum
                {
                    DPLL = 0x0000, // 数字锁相环
                    APLL = 0x0001, // 模拟锁相环
                } Type;
            };

            struct APLL
            {
                struct DIV
                {
                    // 锁相环分频系数
                    typedef enum
                    {
                        DIV_1 = 0x0000,
                        DIV_2_OR_4 = 0x0001 // 如果倍频系数是奇数，那么就是2，如果倍频系数是偶数，那么就是4
                    } Type;
                };

                // 工作模式
                struct Mode
                {
                    typedef enum
                    {
                        DIV = 0x0000, // 分频模式下，VCO被旁路，PLL仅作为分频器，且DIV位失效，此时PLL的分频系数由倍频决定，K为1到15，D=2，K为16，D=4
                        MULT = 0x0001 // 倍频模式下，VCO启用，PLL倍频和分频共同作用
                    } Type;
                };
            };

            struct DPLL
            {
                struct Mode
                {
                    typedef enum
                    {
                        NO_USB_BOOT = 0x2006, // 二分频
                        USB_BOOT = 0x2213 // 四倍频
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
            // 属性
            typedef PLL_Traits<MainPLLTag>::SYSR SYSR;
            typedef PLL_Traits<MainPLLTag>::CLKMD CLKMD;
            // 寄存器
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

        // USB PLL特化
        template<>
        class PLL_Controller<USB_PLLTag>
        {
            // USB PLL属性
            typedef PLL_Traits<USB_PLLTag>::DPLL DPLL;
            typedef PLL_Traits<USB_PLLTag>::APLL APLL;
            typedef PLL_Traits<USB_PLLTag>::SEL SEL;
            // USB PLL寄存器
            typedef mmio::RegAccess<DPLL::REG> DPLL_REG;
            typedef mmio::RegAccess<APLL::REG> APLL_REG;
            typedef mmio::RegAccess<SEL::REG> SEL_REG;

        public:
            template<typename MultType>
            static void set_multiplier(MultType mult)
            {
                APLL_REG::modify_bits(mult, APLL::MULT::MASK, APLL::MULT::OFFSET);
            }

            // 选择APLL或DPLL作为USB PLL的输入
            static void sel(const USB::SEL::Type sel)
            {
                SEL_REG::modify_bits(sel, SEL::PLLSEL::MASK, SEL::PLLSEL::OFFSET);
            }

            // 剩下的东西不写了，写累了，有空再补
            // static bool is_ready()
            // {
            // }
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
        class USB_PLL
        {
        public:
            template<uint16_t Mult>
            static void configure()
            {
                // 剩下的东西不写了，写累了，有空再补
                // Controller::set_multiplier(USBAPLL_Mult::create<Mult>());
                // Controller::enable();
                //
                // while (!Controller::is_ready()); // 等待就绪
            }

        private:
            typedef PLL_Controller<USB_PLLTag> Controller;
        };
    } // namespace pll
} // namespace zq

#endif //ZQ_PLL_H
