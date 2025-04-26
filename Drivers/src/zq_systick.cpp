//
// Created by fairy on 2025/4/26 13:54.
//
// 头文件
#include <zq_systick.h>

namespace zq
{
    namespace systick
    {
        namespace detail
        {
            volatile uint32_t systick=0;
        }
    }
}


// 函数
extern "C"
{
    void interrupt Timer0_ISR()
    {
        ++zq::systick::detail::systick;
    }
}