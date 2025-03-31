#pragma once
#ifndef ASYNC_DELAY_H
#define ASYNC_DELAY_H
#include <zq_conf.h>

/**
 * @brief 异步延迟类
 * @tparam get_tick 获取当前时间函数，基本单位一般为1ms
 * @note 目前思路是，先用这三个16位变量，delay_tick这个是可以换成函数形参的
 *
 */
template<uint16_t(*get_tick)()>
class AsyncDelay
{
public:
    AsyncDelay(): start_tick(0), final_tick(0), delay_tick(0)
    {
    }

    // 需要主动调用
    void init(const uint16_t delay_tick=100)
    {
        set_delay_tick(delay_tick);
        reset();
    }

    void reset() // 重置
    {
        start_tick = get_tick();
        final_tick = start_tick + delay_tick;
    }

    void set_delay_tick(const uint16_t tick)
    {
        delay_tick = tick? tick : 1;
    }

    bool is_timeout(); // 是否超时

private:
    uint16_t start_tick;
    uint16_t final_tick;
    uint16_t delay_tick; // 默认100个tick
};

template<uint16_t(*get_tick)()>
bool AsyncDelay<get_tick>::is_timeout()
{
    // 非阻塞延迟
    if (final_tick < start_tick)
    {
        // 检测溢出情况
        if (get_tick() > final_tick && get_tick() < start_tick)
        {
            // 重置
            start_tick = get_tick();
            final_tick = start_tick + delay_tick;
            // 标志记为1
            return true;
        }
    }
    else
    {
        if (get_tick() > final_tick)
        {
            // 重置
            start_tick = get_tick();
            final_tick = start_tick + delay_tick;
            // 标志记为1
            return true;
        }
    }
    return false;
}


#endif
