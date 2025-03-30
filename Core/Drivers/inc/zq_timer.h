//
// Created by fairy on 2025/3/2 20:09.
//
#ifndef ZQ_TIMER_H
#define ZQ_TIMER_H
#include<zq_conf.h>

// 定时器频率宏
#define TIM_FREQ_200M_to_10M 0,19
#define TIM_FREQ_200M_to_5M 1,19
#define TIM_FREQ_200M_to_2M 3,24
#define TIM_FREQ_200M_to_1M 9,19
#define TIM_FREQ_200M_to_500K 9,39
#define TIM_FREQ_200M_to_250K 9,79
#define TIM_FREQ_200M_to_100K 9,199
#define TIM_FREQ_200M_to_50K 9,399
#define TIM_FREQ_200M_to_25K 9,799
#define TIM_FREQ_200M_to_10K 9,1999

namespace zq
{
    namespace timer
    {
        // 定时器类型
        struct TIMTag
        {
            typedef enum
            {
                TIM0 = 0,
                TIM1 = 0x1400// 后面寄存器均为基址，需要加上偏置0x1400
            } Type;
        };

        DECLARE_REGISTER(TIM, 0x1000);// 主计数器
        DECLARE_REGISTER(PRD, 0x1001);// 预装载计时器
        DECLARE_REGISTER(TCR, 0x1002);// 控制寄存器
        DECLARE_REGISTER(PRSC, 0x1003);// 分频寄存器

        DECLARE_BITS_FIELD_FROM_REG(PRSC, PSC, 0x01C0, 6); // 预分频值bit6~9
        DECLARE_BITS_FIELD_FROM_REG(PRSC, TDDR, 0x0007, 0); // 用于装入PSC中

        namespace TCR
        {
            DECLARE_BITS_FIELD(IDLEEN, 0x8000, 15); // [15] 0:不能进入IDLE状态    1：可以进入IDLE状态，当PERIS为1时进入IDLE状态
            DECLARE_BITS_FIELD(INTEXT, 0x4000, 14); // [14]标志位：时钟源从内部切换为外部标志    0：外部时钟源没有准备好 1：外部时钟源已经准备好
            DECLARE_BITS_FIELD(ERRTIM, 0x2000, 13); // [13]检测错误标志   0: 没有发生错误 1: 发生了错误
            DECLARE_BITS_FIELD(FUNC, 0x1800, 11);   // [11]工作模式选择位
            DECLARE_BITS_FIELD(TLB, 0x0600, 10);    // [10]定时器装载位
            DECLARE_BITS_FIELD(SOFT, 0x0020, 9);    // [9]软件触发位
            DECLARE_BITS_FIELD(FREE, 0x0100, 8);    // [8]与仿真断点有关
            DECLARE_BITS_FIELD(PWID, 0x00C0, 6);    // [6]窄脉冲输出宽度  每当TIM归零时，输出指定宽度的窄脉冲
            DECLARE_BITS_FIELD(ARB, 0x0010, 5);     // [5]自动重装控制位
            DECLARE_BITS_FIELD(TSS, 0x0010, 4);     // [4]定时器停止状态位 0：启动定时器 1：停止定时器
            DECLARE_BITS_FIELD(CP, 0x0008, 3);      // [3]定时器输出时钟/脉冲模式选择   0：脉冲模式  1：时钟模式，占空比固定为50%
            DECLARE_BITS_FIELD(POLAR, 0x0004, 2);   // [2]时钟输出极性位
            DECLARE_BITS_FIELD(DATOUT, 0x0002, 1);  // [1]GPIO模式下，控制引脚输出电平
        }


        // =================================标志===============================
        DECLARE_ATTRIBUTE(Mode,
                          HIZ = 0x00, // TIN/TOUT为高阻态，时钟源是内部CPU时钟源
                          OUTPUT = 0x01, // TIN/TOUT为定时器输出，时钟源是内部CPU时钟
                          GPIO = 0x02, // TIN/TOUT为通用输出，引脚电平反映的是DATOUT位的值
                          EXT_INPUT = 0x03 // TIN/TOUT为定时器输入，时钟源是外部时钟
        );


        template<TIMTag::Type TIMTag>
        class Timer
        {
            // 寄存器
            typedef mmio::RegAccess<TIM::REG + TIMTag> TIM_REG; // 计数寄存器
            typedef mmio::RegAccess<PRD::REG + TIMTag> PRD_REG;
            typedef mmio::RegAccess<TCR::REG + TIMTag> TCR_REG;
            typedef mmio::RegAccess<PRSC::REG + TIMTag> PRSC_REG;

        public:
            /**
             * 初始化
             * @param arr 计数重载值
             * @param psc 预分频系数(0~15)
             * @note 实际频率为 f/[(arr+1)(psc+1)]
             */
            static void init(const uint16_t arr, const uint16_t psc)
            {
                // 停止计时
                stop();

                // 设置预分频系数
                set_prescaler(psc);
                set_prescaler_now(psc);
                // 设置计数重载值
                set_period(arr);

                // 关闭手动重载
                manual_reload<0>();
                // 开启定时器自动重载
                auto_reload<1>();

                // 设置CPU为内部时钟，并且不输出
                set_mode<Mode::HIZ>();

                // 不会进入空闲状态
                set_idle_mode<0>();

                // 启动定时器
                start();
            }

            // 设置period（周期计数值）
            INLINE void set_period(uint16_t arr)
            {
                PRD_REG::write(arr);
            }

            // 设置预分频系数（后续装载）
            INLINE void set_prescaler(uint16_t psc)
            {
                PRSC_REG::modify_bits(psc, PRSC::TDDR::MASK, PRSC::TDDR::SHIFT);
            }

            // 设置当前预分频系数（立即装载）
            INLINE void set_prescaler_now(uint16_t psc)
            {
                PRSC_REG::modify_bits(psc, PRSC::PSC::MASK, PRSC::PSC::SHIFT);
            }

            // 开启定时器自动重载（硬件）
            template<uint16_t mode>
            INLINE void auto_reload()
            {
                TCR_REG::modify_bits(mode, TCR::ARB::MASK, TCR::ARB::SHIFT);
            }

            // 立即手动重载（软件）   0:不重新装载 1：立即重新装载
            // 让PRD、TDDR分别赋值到TIM和PSC中
            template<uint16_t mode>
            INLINE void manual_reload()
            {
                TCR_REG::modify_bits(mode, TCR::TLB::MASK, TCR::TLB::SHIFT);
            }

            // 设置定时器输出模式
            template<Mode::Type mode>
            INLINE void set_mode()
            {
                TCR_REG::modify_bits(mode, TCR::FUNC::MASK, TCR::FUNC::SHIFT);
            }

            // 设置TIN/TOUT的初始极性  0：正极性，从低电平开始，1：负极性，从高电平开始
            template<uint16_t mode>
            INLINE void set_polarity()
            {
                TCR_REG::modify_bits(mode, TCR::POLAR::MASK, TCR::POLAR::SHIFT);
            }

            // 让计时器在遇到仿真断点仍能继续工作
            INLINE void set_simulation_breakpoint()
            {
                // SOFT=0表示仿真暂停时，立即停止
                TCR_REG::modify_bits(0, TCR::SOFT::MASK, TCR::SOFT::SHIFT); // 这个我也不知道是啥，手册上和教材上都它宝贝的不说
                TCR_REG::modify_bits(1, TCR::FREE::MASK, TCR::FREE::SHIFT);
            }

            // 设置空闲时的工作状态 0：定时器不会进入空闲状态 1：如果PERIS为1，那么定时器进入空闲状态
            template<uint16_t mode>
            INLINE void set_idle_mode()
            {
                TCR_REG::modify_bits(mode, TCR::IDLEEN::MASK, TCR::IDLEEN::SHIFT);
            }

            // 启动定时器
            INLINE void start()
            {
                // TSS是停止状态位
                TCR_REG::modify_bits(0, TCR::TSS::MASK, TCR::TSS::SHIFT);
            }

            // 关闭定时器
            INLINE void stop()
            {
                // TSS是停止状态位
                TCR_REG::modify_bits(1, TCR::TSS::MASK, TCR::TSS::SHIFT);
            }
        };

        // ================= 显示实例化 ================
        typedef Timer<TIMTag::TIM0> TIM0;
        typedef Timer<TIMTag::TIM1> TIM1;
    }
}


#endif //ZQ_TIMER_H
