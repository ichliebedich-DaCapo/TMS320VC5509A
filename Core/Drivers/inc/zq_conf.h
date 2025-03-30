//
// Created by fairy on 2025/2/28 19:26.
//
#ifndef ZQ_CONF_H
#define ZQ_CONF_H


#include<stdint.h>

// ======================兼容GCC======================
#if defined(__ARM__)||defined(SIMULATOR)
// 定义关键字
#define ioport
#endif


// ======================兼容GCC======================
// 类型定义


// 强制内联
#define ALWAYS_INLINE __attribute__((always_inline))
#define INLINE static inline

// 辅助宏
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define COMPILE_TIME_ASSERT(pred,msg) \
typedef char CONCATENATE(static_assertion_, __COUNTER__)[(pred) ? 1 : -1]
// // 如果编译器不支持 __COUNTER__，可以使用 __LINE__
// #define COMPILE_TIME_ASSERT_LINE(pred, msg) \
// typedef char CONCATENATE(static_assertion_, __LINE__)[(pred) ? 1 : -1]

// 用于指定寄存器快速声明寄存器的位域
#define DECLARE_BITS_FIELD_FROM_REG(REG_NAME,BITS_NAME, MASK_VAL, SHIFT_VAL) \
namespace REG_NAME {\
    namespace BITS_NAME{\
        enum { \
            MASK = MASK_VAL, \
            SHIFT = SHIFT_VAL \
        };  \
    }\
}
// 直接在寄存器作用域中声明位域
#define DECLARE_BITS_FIELD(BITS_NAME, MASK_VAL, SHIFT_VAL) \
namespace BITS_NAME{\
    enum { \
        MASK = MASK_VAL, \
        SHIFT = SHIFT_VAL \
    };  \
}

// 快速声明整个寄存器（无位域）
// 映射寄存器地址，以便供寄存器类使用 可以通过NAME::REG来访问寄存器地址
#define DECLARE_REGISTER(REG_NAME, ADDRESS) \
namespace REG_NAME { \
    enum { \
        REG = ADDRESS \
    }; \
}

/**
 *快速定义属性类型
 * @example
 *      DECLARE_ATTRIBUTE(Mode,
 *                  HIZ = 0b00, // TIN/TOUT为高阻态，时钟源是内部CPU时钟源
 *                  OUTPUT = 0b01, // TIN/TOUT为定时器输出，时钟源是内部CPU时钟
 *                  GPIO = 0b10, // TIN/TOUT为通用输出，引脚电平反映的是DATOUT位的值
 *                  EXT_INPUT = 0b11 // TIN/TOUT为定时器输入，时钟源是外部时钟
 *      );
 *      使用方法既可以定义这个类型（模拟enum class)，也可以直接通过NAME来访问。比如
 *          void func(NAME::Type mode);
 *          NAME::Type mode = NAME::Type::HIZ;
**/
#define DECLARE_ATTRIBUTE(NAME, ...) \
struct NAME { \
    typedef enum { \
        __VA_ARGS__ \
    } Type; \
}



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

            // 单位操作(操作的是掩码)
            static inline void set_bit(const uint16_t mask)
            {
                *ptr() |= mask;
            }

            static inline void clear_bit(const uint16_t mask)
            {
                *ptr() &= ~mask;
            }

            // 读取位
            static inline bool read_bit(const uint16_t mask)
            {
                return (*ptr() & mask) != 0;
            }

            static inline bool read_bit_not(const uint16_t mask)
            {
                return (*ptr() & mask) == 0;
            }

            /**
             *
             * @param value 要修改的值
             * @param mask 掩码
             * @param shift 位偏移
             */
            static inline void modify_bits(const uint16_t value, const uint16_t mask, const uint16_t shift)
            {
                *ptr() = (*ptr() & ~mask) | ((value << shift) & mask);
            }

            // 直接向寄存器写入16位的值
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
