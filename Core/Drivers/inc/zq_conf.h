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
#define interrupt
#endif
#if !defined(SIMULATOR)
#define nullptr NULL
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

// ======================================== 映射空间 ===========================================
// 把地址映射为存储空间指针，也就是片内寄存器    必须加上ioport关键字
#define REG_MAP(Address) reinterpret_cast<volatile ioport uint16_t *>(Address)
// 片外存储器不能添加ioport和volatile关键字，否则外部存储器不生效
#define EXMEM_MAP(Address) reinterpret_cast<unsigned int *>(Address)
// 根据位宽获取掩码
#define GET_BITS_MASK(WIDTH,SHIFT) (((1U << (WIDTH)) - 1U) << (SHIFT))


namespace hw_registers
{
    // ==================== 标签定义（区分地址类型） ====================
    struct RegTag {
        // 把地址映射为存储空间指针，也就是片内寄存器    必须加上ioport关键字
        typedef volatile ioport uint16_t * MapType;
        static MapType map(const uint32_t addr) { // 处理16位地址
            return reinterpret_cast<MapType>(static_cast<uint16_t>(addr));
        }
    };

    struct ExmemTag {
        // 片外存储器不能添加ioport和volatile关键字，否则外部存储器不生效
        typedef  unsigned int * MapType;
        static MapType map(const uint32_t addr) { // 处理32位地址
            return reinterpret_cast<MapType>(addr);
        }
    };

    // ==================== 基础模板定义 ====================
    template <typename Tag, uint32_t addr, uint16_t mask, uint16_t shift>
    struct BitsField {
        // 通用写操作
        static void write_bits(const uint16_t value) {
            *(Tag::map(addr)) = (*(Tag::map(addr)) & ~mask) | ((value << shift) & mask);
        }

        // 通用读操作
        static uint16_t read_bits() {
            return (*(Tag::map(addr)) & mask) >> shift;
        }
    };



    // ==================== 单比特操作特化（mask=1时触发） ====================
    template <typename Tag, uint32_t addr, uint16_t shift>
    struct BitsField<Tag, addr, 1, shift> {
        // 置位
        static void set() {
            *(Tag::map(addr)) |= (1U << shift);
        }

        // 清除
        static void clear() {
            *(Tag::map(addr)) &= ~(1U << shift);
        }

        static void write_bit(const bool value)
        {
            *(Tag::map(addr)) = (*(Tag::map(addr)) & ~(1U << shift)) | (value << shift);
        }

        // 读取状态
        static bool read_bit() {
            return (*(Tag::map(addr)) & (1U << shift)) != 0;
        }
        static bool read_bit_not()
        {
            return (*(Tag::map(addr)) & (1U << shift)) == 0;
        }
    };
}


// ======================================== 命名空间法（可合并） ===========================================
// ============== 寄存器声明 ==============
#define DECLARE_REGISTER(REG_NAME, ADDRESS)\
namespace REG_NAME\
{\
enum {REG = ADDRESS};\
INLINE void write(uint16_t value) {*REG_MAP(ADDRESS) = value;}\
INLINE uint16_t read() {return *REG_MAP(ADDRESS);}\
}

// 单位字段声明(必须在寄存器声明之后)
#define DECLARE_REG_BIT(REG_NAME,BITS_NAME, SHIFT)\
namespace REG_NAME\
{\
typedef hw_registers::BitsField<hw_registers::RegTag,REG_NAME::REG,1,SHIFT> BITS_NAME;\
}

// 多位字段声明(必须在寄存器声明之后)
#define DECLARE_REG_FIELD(REG_NAME,BITS_NAME, WIDTH, SHIFT)\
namespace REG_NAME\
{\
typedef hw_registers::BitsField<hw_registers::RegTag,REG_NAME::REG,GET_BITS_MASK(WIDTH,SHIFT),SHIFT> BITS_NAME;\
}

// ============== 外部存储器声明（或者一些特殊寄存器） ==============
#define DECLARE_EXEMEMORY(REG_NAME, ADDRESS)\
namespace REG_NAME\
{\
enum {REG = ADDRESS};\
INLINE void write(uint16_t value) {*EXMEM_MAP(ADDRESS) = value;}\
INLINE uint16_t read() {return *EXMEM_MAP(ADDRESS);}\
}

// 单位字段声明(必须在寄存器声明之后)
#define DECLARE_EXEMEM_BIT(REG_NAME,BITS_NAME, SHIFT)\
namespace REG_NAME\
{\
typedef hw_registers::BitsField<hw_registers::ExmemTag,REG_NAME::REG,1,SHIFT> BITS_NAME;\
}

// 多位字段声明(必须在寄存器声明之后)
#define DECLARE_EXEMEM_FIELD(REG_NAME,BITS_NAME, WIDTH, SHIFT)\
namespace REG_NAME\
{\
typedef hw_registers::BitsField<hw_registers::ExmemTag,REG_NAME::REG,GET_BITS_MASK(WIDTH,SHIFT),SHIFT> BITS_NAME;\
}

// ========================================寄存器映射（模板类法）===========================================


    // 寄存器声明
    template<uint16_t address>
    struct RegisterAccess
    {
        // 单位操作(操作的是掩码)
        INLINE void write(const uint16_t shift)
        {
            *REG_MAP(address) |=(1<<shift);
        }

        INLINE void write(const uint16_t shift,const bool data)
        {
            *REG_MAP(address) =(*REG_MAP(address) & ~(1<<shift)) |((data << shift) & (1<<shift));
        }

        INLINE uint16_t read()
        {
            return *REG_MAP(address);
        }

        INLINE bool read(const uint16_t shift)
        {
            return ((*REG_MAP(address)) & (1<<shift)) != 0;
        }

        INLINE void clear()
        {
            *REG_MAP(address) = 0;
        }

        // 单位操作(操作的是掩码)
        INLINE void clear(const bool shift)
        {
            *REG_MAP(address) &= ~(1<<shift);
        }
    };



    // ======== 寄存器声明 ========
#define DECLARE_REGISTER_T(REG_NAME,ADDRESS)\
struct REG_NAME:RegisterAccess<(ADDRESS)>{};

// 两者不能合并
#define BEGIN_REG_T(REG_NAME, ADDRESS)\
struct REG_NAME:RegisterAccess<(ADDRESS)>\
{\
enum\
{\
REG = ADDRESS\
};

    // 单位字段声明，必须放在前后的宏之间
#define DECLARE_BIT_FIELD_T(BIT_NAME, SHIFT)\
struct BIT_NAME: hw_registers::BitsField<hw_registers::RegTag,REG,1,SHIFT>\
{};

    // 多位字段声明，必须放在前后的宏之间
#define DECLARE_BITS_FIELD_T(BITS_NAME, WIDTH, SHIFT)\
struct BITS_NAME: hw_registers::BitsField<hw_registers::RegTag,REG,GET_BITS_MASK(WIDTH,SHIFT),SHIFT>\
{};

    // ========寄存器结束========
#define END_REG_T()\
};


    // ===============================外部存储器===================================
    // 32位的地址
    template<uint32_t address>
    struct ExMemAccess
    {

        INLINE void write(const uint16_t value)
        {
            *EXMEM_MAP(address) =value;
        }

        // 单位操作,把该位变为1
        INLINE void write(const uint16_t shift,const bool data)
        {
            *EXMEM_MAP(address) =(*EXMEM_MAP(address) & ~(1<<shift)) |((data << shift) & (1<<shift));
        }

        INLINE uint16_t read()
        {
            return *EXMEM_MAP(address);
        }

        // 返回的是单位的值
        INLINE bool read(const uint16_t shift)
        {
            return ((*EXMEM_MAP(address)) & (1<<shift)) != 0;
        }

        // 翻转
        INLINE void toggle(const uint16_t shift)
        {
            *EXMEM_MAP(address) ^= (1<<shift);
        }

        INLINE void clear()
        {
            *EXMEM_MAP(address) = 0;
        }

        // 单位操作(操作的是掩码)
        INLINE void clear(const uint16_t shift)
        {
            *EXMEM_MAP(address) &= ~(1<<shift);
        }
    };
    // 外部存储器寄存器声明
#define DECLARE_EXMEM_REGISTER_T(REG_NAME,ADDRESS)\
struct REG_NAME:ExMemAccess<ADDRESS> {};


// =====================辅助内联函数====================
INLINE void swap(uint16_t &a, uint16_t &b)
{
    const uint16_t t = a;
    a = b;
    b = t;
}

INLINE uint16_t min(const uint16_t &a, const uint16_t &b)
{
    return a < b ? a : b;
}

INLINE uint16_t max(const uint16_t &a, const uint16_t &b)
{
    return a > b ? a : b;
}

// 绝对值差
INLINE uint16_t abs_diff(const uint16_t &a, const uint16_t &b)
{
    return ((a) > (b) ? (a) - (b) : (b) - (a));
}

#endif //ZQ_CONF_H
