//
// Created by fairy on 2025/4/26 13:54.
//
#ifndef ZQ_SYSTICK_H
#define ZQ_SYSTICK_H
#include <zq_conf.h>
#include "zq_timer.h"

namespace zq {
    namespace systick {
        namespace detail {
            extern volatile uint32_t systick;
            // 定时器配置常量
            const uint32_t TIM_PRESCALER = 15;
            const uint32_t TIM_PERIOD = 11999;
            const uint32_t CYCLES_PER_US = 12;          // 12 cycles/μs
            const uint32_t TOTAL_CYCLES_PER_MS = 192000; // 12000*16=192000 cycles
        }

        inline uint32_t get_tick(){return detail::systick;}
        inline void init()
        {
            timer::Timer0::init(TIM_FREQ_192M_to_1K);
            timer::Timer0::start_IT();
        }

        class Delay {
            typedef timer::detail::TIM<timer::detail::Offset::TIM0> TIM_REG;
            typedef timer::detail::PRSC<timer::detail::Offset::TIM0>::PSC PSC_REG;

        public:
            // 阻塞式毫秒延时
            static void ms(const uint32_t _ms) {
                const uint32_t start = get_tick();
                while ((get_tick() - start) < _ms) { /* 空循环等待 溢出自动处理 */}
            }

            // 阻塞式微秒延时（支持超过1ms的值）
            static void us(uint32_t us) {
                if (us == 0) return;

                // 处理超过1ms的部分
                if (us >= 1000) {
                    ms(us / 1000);
                    us %= 1000;
                    if (us == 0) return;
                }

                const uint32_t cycles_needed = us * detail::CYCLES_PER_US;
                const uint32_t timer_period_cycles = detail::TOTAL_CYCLES_PER_MS;

                uint32_t tim_start, psc_start;
                do { // 原子读取 TIM 和 PSC
                    tim_start = TIM_REG::read();
                    psc_start = PSC_REG::read_bits();
                } while (TIM_REG::read() != tim_start || PSC_REG::read_bits() != psc_start);

                const uint32_t start_cycles = (detail::TIM_PERIOD - tim_start) * (detail::TIM_PRESCALER + 1)
                                            + (detail::TIM_PRESCALER - psc_start);

                while (true) {
                    uint32_t tim_now, psc_now;
                    do { // 再次原子读取
                        tim_now = TIM_REG::read();
                        psc_now = PSC_REG::read_bits();
                    } while (TIM_REG::read() != tim_now || PSC_REG::read_bits() != psc_now);

                    const uint32_t current_cycles = (detail::TIM_PERIOD - tim_now) * (detail::TIM_PRESCALER + 1)
                                                  + (detail::TIM_PRESCALER - psc_now);

                    uint32_t difference;
                    if (current_cycles >= start_cycles) {
                        difference = current_cycles - start_cycles;
                    } else {
                        difference = (timer_period_cycles - start_cycles) + current_cycles;
                    }

                    if (difference >= cycles_needed) break;
                }
            }
        };

        // 非阻塞式循环定时器（溢出安全）
        class AsyncDelay {
            uint32_t next_target_;  // 下一次超时时间点
            uint32_t interval_;      // 定时间隔（单位：tick）
            bool is_active_;         // 定时器激活标志

        public:
            AsyncDelay() : next_target_(0), interval_(0), is_active_(false) {}

            // 启动/重启定时器（单位：毫秒）
            void start(const uint32_t ms) {
                interval_ = ms;
                if (interval_ == 0) {
                    is_active_ = false;
                    return;
                }
                is_active_ = true;
                next_target_ = get_tick() + interval_;
            }

            // 停止定时器
            void stop() {
                is_active_ = false;
            }

            // 检查是否超时（必须循环调用）
            bool is_timeout() {
                if (!is_active_) return false;

                /* 溢出安全的时间差比较 */
                const uint32_t current = get_tick();
                const int32_t diff = static_cast<int32_t>(current - next_target_);

                if (diff >= 0) {  // 当前时间 >= 目标时间（处理溢出）
                    /* 更新下一个目标时间（固定间隔，避免累积误差） */
                    next_target_ += interval_;
                    return true;
                }
                return false;
            }

            // 剩余时间查询（单位：tick）
            uint32_t remaining() const {
                if (!is_active_) return 0;
                const uint32_t current = get_tick();
                return (next_target_ - current) + (current > next_target_ ? 0xFFFFFFFF : 0);
            }
        };
    }
}

#endif //ZQ_SYSTICK_H
