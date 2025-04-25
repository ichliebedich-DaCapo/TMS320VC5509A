//
// Created by fairy on 2025/3/2 20:09.
//
#ifndef ZQ_TIMER_H
#define ZQ_TIMER_H
#include<zq_conf.h>
#include <zq_interrupt.h>
// 定时器频率宏
/* 分频公式：F_out = 192MHz / ((psc+1) * (arr+1)) */
//---------------------------------------------------------------------------------------------------------
#define TIM_FREQ_192M_to_10M     0,18    // 10.105MHz (+1.05%)  (0+1)*(18+1)=19 → 192M/19≈10.105M
#define TIM_FREQ_192M_to_5M      1,18    // 5.0526MHz (+1.05%)  (1+1)*(18+1)=38 → 192M/38≈5.0526M
#define TIM_FREQ_192M_to_2M     11, 7    // 2MHz (0%)           (11+1)*(7+1)=96 → 192M/96=2M
#define TIM_FREQ_192M_to_1M     15,11    // 1MHz (0%)           (15+1)*(11+1)=192 → 192M/192=1M
#define TIM_FREQ_192M_to_500K   15,23    // 500KHz (0%)         (15+1)*(23+1)=384 → 192M/384=500K
#define TIM_FREQ_192M_to_200K   15,59    // 200KHz (0%)         (15+1)*(59+1)=960 → 192M/960=200K
#define TIM_FREQ_192M_to_100K   15,119   // 100KHz (0%)         (15+1)*(119+1)=1920 → 192M/1920=100K
#define TIM_FREQ_192M_to_50K    15,239   // 50KHz (0%)          (15+1)*(239+1)=3840 → 192M/3840=50K
#define TIM_FREQ_192M_to_48K     9,399    // 48.0kHz (0%误差)       (9 * 40=360 → 192/360=0.48M)
#define TIM_FREQ_192M_to_20K    15,599   // 20KHz (0%)          (15+1)*(599+1)=9600 → 192M/9600=20K
#define TIM_FREQ_192M_to_10K    15,1199  // 10KHz (0%)          (15+1)*(1199+1)=19200 → 192M/19200=10K
#define TIM_FREQ_192M_to_5K     15,2399  // 5KHz (0%)           (15+1)*(2399+1)=38400 → 192M/38400=5K
#define TIM_FREQ_192M_to_2K     15,5999  // 2KHz (0%)           (15+1)*(5999+1)=96000 → 192M/96000=2K
#define TIM_FREQ_192M_to_1K     15,11999 // 1KHz (0%)           (15+1)*(11999+1)=192000 → 192M/192000=1K
#define TIM_FREQ_192M_to_500HZ  15,23999 // 500Hz (0%)          (15+1)*(23999+1)=384000 → 192M/384000=500
#define TIM_FREQ_192M_to_200HZ  15,59999 // 200Hz (0%)          (15+1)*(59999+1)=960000 → 192M/960000=200
#define TIM_FREQ_192M_to_100HZ  15,65535 // 183.1Hz (实际目标183Hz)  (15+1)*65536=1048576 → 192M/1048576≈183.1Hz

namespace zq
{
    namespace timer
    {
        namespace detail
        {
            // ========定时器寄存器========
            // 主计数器
            template<uint16_t offset>
            DECLARE_REGISTER_T(TIM, 0x1000+offset);

            // 预装载计时器
            template<uint16_t offset>
            DECLARE_REGISTER_T(PRD, 0x1001+offset);

            // TCR寄存器位域
            template<uint16_t offset>
            BEGIN_REG_T(TCR, 0x1002+offset) // 控制寄存器
                DECLARE_BIT_FIELD_T(IDLEEN, 15) // [15] 0:不能进入IDLE状态    1：可以进入IDLE状态，当PERIS为1时进入IDLE状态
                DECLARE_BIT_FIELD_T(INTEXT, 14) // [14]标志位：时钟源从内部切换为外部标志    0：外部时钟源没有准备好 1：外部时钟源已经准备好
                DECLARE_BIT_FIELD_T(ERRTIM, 13) // [13]检测错误标志   0: 没有发生错误 1: 发生了错误
                DECLARE_BITS_FIELD_T(FUNC, 2, 11) // [11~12]工作模式选择位
                DECLARE_BIT_FIELD_T(TLB, 10) // [10]定时器装载位
                DECLARE_BIT_FIELD_T(SOFT, 9) // [9]软件触发位
                DECLARE_BIT_FIELD_T(FREE, 8) // [8]与仿真断点有关
                DECLARE_BITS_FIELD_T(PWID, 2, 6) // [6~7]窄脉冲输出宽度  每当TIM归零时，输出指定宽度的窄脉冲
                DECLARE_BIT_FIELD_T(ARB, 5) // [5]自动重装控制位
                DECLARE_BIT_FIELD_T(TSS, 4) // [4]定时器停止状态位 0：启动定时器 1：停止定时器
                DECLARE_BIT_FIELD_T(CP, 3) // [3]定时器输出时钟/脉冲模式选择   0：脉冲模式  1：时钟模式，占空比固定为50%
                DECLARE_BIT_FIELD_T(POLAR, 2) // [2]时钟输出极性位
                DECLARE_BIT_FIELD_T(DATOUT, 1) // [1]GPIO模式下，控制引脚输出电平
            END_REG_T()

            // PRSC寄存器位域
            template<uint16_t offset>
            BEGIN_REG_T(PRSC, 0x1003+offset) // 分频寄存器
                DECLARE_BITS_FIELD_T(PSC, 4, 6) // 预分频值bit6~9
                DECLARE_BITS_FIELD_T(TDDR, 4, 0) // 用于装入PSC中
            END_REG_T()


            // =================================标志===============================
            DECLARE_ATTRIBUTE(Mode,
                              HIZ = 0x00, // TIN/TOUT为高阻态，时钟源是内部CPU时钟源
                              OUTPUT = 0x01, // TIN/TOUT为定时器输出，时钟源是内部CPU时钟
                              GPIO = 0x02, // TIN/TOUT为通用输出，引脚电平反映的是DATOUT位的值
                              EXT_INPUT = 0x03 // TIN/TOUT为定时器输入，时钟源是外部时钟
            );

            // 定时器偏移量
            DECLARE_ATTRIBUTE(Offset, TIM0 = 0, TIM1=0x1400);
        }
        // =============================== 定时器模板类 ===============================
        /**
         * @brief 通用定时器模板类
         * @tparam offset 定时器偏移量
         * @detail 提供统一的定时器操作接口，通过模板参数区分不同定时器实例
         */
        template<detail::Offset::Type offset>
        class Timer
        {
            // 寄存器映射
            // typedef TIM<offset> TIM_REG;
            typedef detail::PRD<offset> PRD_REG;
            typedef detail::PRSC<offset> PRSC_REG;
            typedef detail::TCR<offset> TCR_REG;

        public:
            /**
             * @brief 初始化定时器(初始化后自动开启)
             * @param psc 预分频系数(0-15)
             * @param arr 自动重装载值(0-65535)
             * @note 实际频率计算公式：f = f_input / [(arr+1)(psc+1)]
             */
            static void init(const uint16_t psc, const uint16_t arr)
            {
                stop(); // 先停止定时器
                set_prescaler(psc); // 设置分频重载值
                set_period(arr); // 设置周期值
                set_simulation_breakpoint();
                manual_reload<0>(); // 禁用立即重载
                auto_reload<1>(); // 启用自动重装
                TCR_REG::CP::set();// 时钟模式，输出方波
                set_polarity<1>();// 设置为正则极性
                set_mode<detail::Mode::OUTPUT>(); // 默认高阻模式
                set_idle_mode<0>(); // 禁用空闲模式
                start(); // 启动定时器

                // TCR_REG::TSS::set();// 先停止
                // TCR_REG::TLB::set();// 重新装载
                // TCR_REG::IDLEEN::clear();// 禁用空闲模式
                // PRSC_REG::TDDR::write_bits(psc);
                // PRD_REG::write(arr);
                // TCR_REG::FUNC::write_bits(0x01);// 工作模式
                //
                // TCR_REG::ARB::set();// 自动重装
                // TCR_REG::CP::set();// 时钟模式，输出方波
                // TCR_REG::TSS::clear();// 启动
                // TCR_REG::TLB::clear();// 禁止自动加载

            }

            /** 设置周期值（自动重装载值） */
            INLINE void set_period(uint16_t arr)
            {
                PRD_REG::write(arr);
            }

            /** 设置预分频重载值*/
            INLINE void set_prescaler(const uint16_t psc)
            {
                PRSC_REG::TDDR::write_bits(psc);
                // PRSC_REG::PSC::write_bits(psc);// 当前计数器也修改
            }

            /** 自动重装载控制（0:禁用 1:启用） */
            template<uint16_t mode>
            INLINE void auto_reload()
            {
                TCR_REG::ARB::write_bit(mode);
            }

            /** 立即触发重装载（0:正常 1:立即重载） */
            template<uint16_t mode>
            INLINE void manual_reload()
            {
                TCR_REG::TLB::write_bit(mode);
            }

            /** 设置定时器工作模式 */
            template<detail::Mode::Type mode>
            INLINE void set_mode()
            {
                TCR_REG::FUNC::write_bits(mode);
            }

            /** 设置输出极性（0:正极性 1:负极性） */
            template<uint16_t mode>
            INLINE void set_polarity()
            {
                TCR_REG::POLAR::write_bit(mode);
            }

            /** 配置仿真断点行为 */
            INLINE void set_simulation_breakpoint()
            {
                TCR_REG::SOFT::clear(); // 仿真暂停时立即停止
                TCR_REG::FREE::set(); // 自由运行模式
            }

            /** 设置空闲模式（0:保持运行 1:进入空闲） */
            template<uint16_t mode>
            INLINE void set_idle_mode()
            {
                TCR_REG::IDLEEN::write_bit(mode);
            }

            /** 启动定时器 */
            INLINE void start()
            {
                TCR_REG::TSS::clear(); // 清除停止位
            }

            /** 停止定时器 */
            INLINE void stop()
            {
                TCR_REG::TSS::set(); // 设置停止位
            }

            // 开启中断
            INLINE void start_IT()
            {
                // 没有constexpr很麻烦
                if (offset ==0 )
                {
                    cpu::IER0::TINT0::set();
                    cpu::DBIER0::TINT0::set();
                } else
                {
                    cpu::IER1::TINT1::set();
                    cpu::DBIER1::TINT1::set();
                }
            }

            INLINE void stop_IT()
            {
                if (offset ==0 )
                {
                    cpu::IER0::TINT0::clear();
                    cpu::DBIER0::TINT0::clear();
                } else
                {
                    cpu::IER1::TINT1::clear();
                    cpu::DBIER1::TINT1::clear();
                }
            }
        };

        // =============================== 定时器实例化 ===============================
        typedef Timer<detail::Offset::TIM0> Timer0;
        typedef Timer<detail::Offset::TIM1> Timer1;
    }
}


#endif //ZQ_TIMER_H
