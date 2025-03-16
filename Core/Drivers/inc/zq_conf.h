//
// Created by fairy on 2025/2/28 19:26.
//
#ifndef ZQ_CONF_H
#define ZQ_CONF_H


#include<stdint.h>

// ======================兼容GCC======================
#ifdef __ARM__
// 定义关键字
#define ioport
#endif
// ======================兼容GCC======================
// 类型定义


// 强制内联
#define ALWAYS_INLINE __attribute__((always_inline))
#define INLINE static inline


namespace zq
{
    namespace mmio
    {
        template<uint16_t Address>
        struct RegAccess
        {
            // 编译期地址计算
            static inline volatile ioport uint16_t *ptr()
            {
                return reinterpret_cast<volatile ioport uint16_t *>(Address);
            }

            // 通用位操作
            static inline void set_bits(const uint16_t mask)
            {
                *ptr() |= mask;
            }

            static inline void clear_bits(const uint16_t mask)
            {
                *ptr() &= ~mask;
            }

            static inline void write(const uint16_t value)
            {
                *ptr() = value;
            }

            static inline uint16_t read()
            {
                return *ptr();
            }
        };
    } // namespace mmio
} // namespace hal


#endif //ZQ_CONF_H
