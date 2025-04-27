#ifndef ZQ_FONT_5X8_H
#define ZQ_FONT_5X8_H
#include<zq_font_base.h>

namespace GUI
{
    namespace Font
    {
        // 所有字模数据数组定义
        namespace detail
        {
            const unsigned char fonts_10x8_0[] = { 0x1E, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1E };
            const unsigned char fonts_10x8_1[] = { 0x03, 0x0D, 0x09, 0x11, 0x11, 0x09, 0x0D, 0x03 };
            const unsigned char fonts_10x8_2[] = { 0x1F, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x1F };
            const unsigned char fonts_10x8_3[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1F };
            const unsigned char fonts_10x8_4[] = { 0x11, 0x13, 0x13, 0x15, 0x15, 0x1D, 0x19, 0x11 };
            const unsigned char fonts_10x8_5[] = { 0x04, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04 };  // 每个数组占一行
        }
        
        const FontChar fonts_10x8[] = {
            // "C"
        {
            .name = "C",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_0
        },// "D"
        {
            .name = "D",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_1
        },// "E"
        {
            .name = "E",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_2
        },// "L"
        {
            .name = "L",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_3
        },// "N"
        {
            .name = "N",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_4
        },// "O"
        {
            .name = "O",
            .width = 5,
            .height = 8,
            .data = detail::fonts_10x8_5
        },
            // 结束标记
            {
                .name = nullptr,
                .width = 0,
                .height = 0,
                .data = nullptr
            },
        };
    }
}
#endif
