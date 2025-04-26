//
// Created by fairy on 2025/3/31 08:51.
//
#ifndef FLAG_HPP
#define FLAG_HPP

#include<zq_conf.h>

/**
 * 标志操作（实际上只有16位）
 * @note 把标志以bit的形式封装在枚举体里，这样就可以很方便地进行标志操作了
 *       DECLARE_ATTRIBUTE(Wave,
 *           state = 0,// 状态
 *           auto_reset,// 是否自动重启
 *           trigger// 触发标志
 *       );
 */
class FlagOperate
{
    uint16_t flag_;

public:
    FlagOperate(): flag_(0)
    {
    }

    // 获取标志位
    bool get(const uint16_t flag) const
    {
        return (flag_ & flag) ? true : false;
    }

    // 设置标志位
    void set(const uint16_t flag)
    {
        flag_ |= flag;
    }

    // 清除标志位
    void clear(const uint16_t flag)
    {
        flag_ &= ~flag;
    }

    // 取反标志位
    void toggle(const uint16_t flag)
    {
        flag_ ^= flag;
    }

    // 清除所有标志位
    void clear()
    {
        flag_ = 0;
    }
};


#endif //FLAG_HPP
