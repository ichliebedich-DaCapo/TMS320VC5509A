//
// Created by fairy on 2025/2/28 19:26.
//
#ifndef ZQ_CONF_H
#define ZQ_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
#include<stdint.h>

// ======================兼容GCC======================
#ifdef __GNUC__
// 定义关键字
#define ioport
#endif
// ======================兼容GCC======================

// 强制内联
#define ALWAYS_INLINE __attribute__((always_inline))
#define INLINE static inline


#ifdef __cplusplus
}
#endif
#endif //ZQ_CONF_H
