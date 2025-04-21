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
        // ========定时器寄存器========
        template<uint16_t offset>
        DECLARE_REGISTER_T(TIM, 0x1000+offset); // 主计数器

        template<uint16_t offset>
        DECLARE_REGISTER_T(PRD, 0x1001+offset); // 预装载计时器

        // TCR寄存器位域
        template<uint16_t offset>
        BEGIN_REG_T(TCR, 0x1002+offset); // 控制寄存器
        DECLARE_BIT_FIELD_T(IDLEEN, 15); // [15] 0:不能进入IDLE状态    1：可以进入IDLE状态，当PERIS为1时进入IDLE状态
        DECLARE_BIT_FIELD_T(INTEXT, 14); // [14]标志位：时钟源从内部切换为外部标志    0：外部时钟源没有准备好 1：外部时钟源已经准备好
        DECLARE_BIT_FIELD_T(ERRTIM, 13); // [13]检测错误标志   0: 没有发生错误 1: 发生了错误
        DECLARE_BIT_FIELD_T(FUNC, 11); // [11]工作模式选择位
        DECLARE_BIT_FIELD_T(TLB, 10); // [10]定时器装载位
        DECLARE_BIT_FIELD_T(SOFT, 9); // [9]软件触发位
        DECLARE_BIT_FIELD_T(FREE, 8); // [8]与仿真断点有关
        DECLARE_BIT_FIELD_T(PWID, 6); // [6]窄脉冲输出宽度  每当TIM归零时，输出指定宽度的窄脉冲
        DECLARE_BIT_FIELD_T(ARB, 5); // [5]自动重装控制位
        DECLARE_BIT_FIELD_T(TSS, 4); // [4]定时器停止状态位 0：启动定时器 1：停止定时器
        DECLARE_BIT_FIELD_T(CP, 3); // [3]定时器输出时钟/脉冲模式选择   0：脉冲模式  1：时钟模式，占空比固定为50%
        DECLARE_BIT_FIELD_T(POLAR, 2); // [2]时钟输出极性位
        DECLARE_BIT_FIELD_T(DATOUT, 1); // [1]GPIO模式下，控制引脚输出电平
        END_REG_T()

        // PRSC寄存器位域
        template<uint16_t offset>
        BEGIN_REG_T(PRSC, 0x1003+offset);// 分频寄存器
        DECLARE_BITS_FIELD_T(PSC, 4, 6); // 预分频值bit6~9
        DECLARE_BITS_FIELD_T(TDDR, 3, 0); // 用于装入PSC中
        END_REG_T()


        // =================================标志===============================
        DECLARE_ATTRIBUTE(Mode,
                          HIZ = 0x00, // TIN/TOUT为高阻态，时钟源是内部CPU时钟源
                          OUTPUT = 0x01, // TIN/TOUT为定时器输出，时钟源是内部CPU时钟
                          GPIO = 0x02, // TIN/TOUT为通用输出，引脚电平反映的是DATOUT位的值
                          EXT_INPUT = 0x03 // TIN/TOUT为定时器输入，时钟源是外部时钟
        );

        // 定时器偏移量
        DECLARE_ATTRIBUTE(Offset,TIM0 = 0,TIM1=0x1400);


        // =============================== 定时器模板类 ===============================
        /**
         * @brief 通用定时器模板类
         * @tparam offset 定时器偏移量
         * @detail 提供统一的定时器操作接口，通过模板参数区分不同定时器实例
         */
        template<Offset::Type offset>
        class Timer
        {
            // 寄存器映射
            typedef TIM<offset> TIM_REG;
            typedef PRD<offset> PRD_REG;
            typedef PRSC<offset> PRSC_REG;
            typedef TCR<offset> TCR_REG;

        public:
            /**
             * @brief 初始化定时器
             * @param arr 自动重装载值
             * @param psc 预分频系数(0-15)
             * @note 实际频率计算公式：f = f_input / [(arr+1)(psc+1)]
             */
            static void init(const uint16_t arr, const uint16_t psc)
            {
                stop(); // 先停止定时器
                set_prescaler(psc); // 设置分频重载值
                set_prescaler_now(psc); // 立即应用分频值
                set_period(arr); // 设置周期值
                set_simulation_breakpoint();
                manual_reload<0>(); // 禁用立即重载
                auto_reload<1>(); // 启用自动重装
                set_mode<Mode::HIZ>(); // 默认高阻模式
                set_idle_mode<0>(); // 禁用空闲模式
                start(); // 启动定时器
            }

            /** 设置周期值（自动重装载值） */
            INLINE void set_period(uint16_t arr)
            {
                PRD_REG::write(arr);
            }

            /** 设置预分频重载值（下次生效） */
            INLINE void set_prescaler(const uint16_t psc)
            {
                PRSC_REG::TDDR::write_bits(psc);
            }

            /** 立即更新预分频值 */
            INLINE void set_prescaler_now(const uint16_t psc)
            {
                PRSC_REG::PSC::write_bits(psc);
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
            template<Mode::Type mode>
            INLINE void set_mode()
            {
                TCR_REG::FUNC::write_bit(mode);
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
                TCR_REG::SOFT::clear_bit(); // 仿真暂停时立即停止
                TCR_REG::FREE::set_bit(); // 自由运行模式
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
                TCR_REG::TSS::clear_bit(); // 清除停止位
            }

            /** 停止定时器 */
            INLINE void stop()
            {
                TCR_REG::TSS::set_bit(); // 设置停止位
            }
        };

        // =============================== 定时器实例化 ===============================
        typedef Timer<Offset::TIM0> Timer0;
        typedef Timer<Offset::TIM1> Timer1;



    }


}


#endif //ZQ_TIMER_H
