//
// Created by fairy on 2025/3/1 16:11.
//
// 头文件
#include<zq_conf.h>

// 函数

// 为了兼容GCC，需要定义_exit函数
#ifdef __GNUC__
void _exit(int status)
{
    while (1)
    {
    }
}
#endif