//
// Created by fairy on 2025/3/1 23:57.
//
#include "zq_delay.h"
// 头文件


// 函数
void delay(const uint16_t nDelay)
{
    int kk=0;
    for ( int ii = 0;ii<nDelay;ii++ )
    {
        for ( int jj = 0;jj<1024;jj++ )
        {
            kk++;
        }
    }
}