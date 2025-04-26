//
// Created by fairy on 2025/4/26 13:54.
//
#ifndef ZQ_SYSTICK_H
#define ZQ_SYSTICK_H
#include <zq_conf.h>
#include "zq_timer.h"

namespace zq
{
    namespace systick
    {
        namespace detail
        {
            extern volatile uint32_t systick;
            // 定时器配置常量
            const uint32_t TIM_PRESCALER = 15;
            const uint32_t TIM_PERIOD = 11999;
            const uint32_t CYCLES_PER_US = 12; // 12 cycles/μs
            const uint32_t TOTAL_CYCLES_PER_MS = 192000; // 12000*16=192000 cycles
        }

        inline uint32_t get_tick() { return detail::systick; }

        inline void init()
        {
            timer::Timer0::init(TIM_FREQ_192M_to_1K);
            timer::Timer0::start_IT();
        }

        class Delay
        {
            typedef timer::detail::TIM<timer::detail::Offset::TIM0> TIM_REG;
            typedef timer::detail::PRSC<timer::detail::Offset::TIM0>::PSC PSC_REG;

        public:
            // 阻塞式毫秒延时
            static void ms(const uint32_t _ms)
            {
                const uint32_t start = get_tick();
                while ((get_tick() - start) < _ms){}/* 空循环等待 溢出自动处理 */
            }

            // 阻塞式微秒延时（只能输入1ms以内，超过范围请自觉切换）
            static void us(const uint16_t us)
            {
                const uint16_t start = TIM_REG::read();
                const uint16_t delay_ticks = us * 12; // 每个 us 对应 12 个 TIM 周期
                uint32_t elapsed_ticks;

                do {
                    const uint16_t current = TIM_REG::read();
                    // 处理溢出：若 current > start，说明 TIM 已重载
                    if (current > start) {
                        elapsed_ticks = (start + 12000) - current; // 12000 = 11999 +1
                    } else {
                        elapsed_ticks = start - current;
                    }
                } while (elapsed_ticks < delay_ticks);

                // // 下面这个延时有问题
                // const uint16_t start = TIM_REG::read();
                // const uint16_t delay_ticks = us * 12;  // 12个TIM周期=1us
                // // 利用无符号数溢出特性自动处理定时器递减和重载
                // while ( static_cast<uint16_t>(start - TIM_REG::read()) < delay_ticks ) {}
            }
        };

        // 非阻塞式循环定时器（溢出安全）
        class AsyncDelay
        {
            uint32_t next_target_; // 下一次超时时间点
            uint32_t interval_; // 定时间隔（单位：tick）
            bool is_active_; // 定时器激活标志

        public:
            AsyncDelay() : next_target_(0), interval_(0), is_active_(false) {}

            // 启动/重启定时器（单位：毫秒）
            void start(const uint32_t ms)
            {
                interval_ = ms;
                if (interval_ == 0)
                {
                    is_active_ = false;
                    return;
                }
                is_active_ = true;
                next_target_ = get_tick() + interval_;
            }

            // 停止定时器
            void stop()
            {
                is_active_ = false;
            }

            // 检查是否超时（必须循环调用）
            bool is_timeout()
            {
                if (!is_active_) return false;

                /* 溢出安全的时间差比较 */
                const uint32_t current = get_tick();
                const int32_t diff = static_cast<int32_t>(current - next_target_);

                if (diff >= 0)
                {
                    // 当前时间 >= 目标时间（处理溢出）
                    /* 更新下一个目标时间（固定间隔，避免累积误差） */
                    next_target_ += interval_;
                    return true;
                }
                return false;
            }

            // 剩余时间查询（单位：tick）
            uint32_t remaining() const
            {
                if (!is_active_) return 0;
                const uint32_t current = get_tick();
                return (next_target_ - current) + (current > next_target_ ? 0xFFFFFFFF : 0);
            }
        };
    }
}

#endif //ZQ_SYSTICK_H
