#ifndef ZQ_FONT_5X10_H
#define ZQ_FONT_5X10_H
#include<zq_font_base.h>

namespace GUI
{
    namespace Font
    {
        // 所有字模数据数组定义
        namespace detail
        {
            const unsigned char fonts_10x10_0[] = { 0x00, 0x00, 0x06, 0x09, 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 };
            const unsigned char fonts_10x10_1[] = { 0x00, 0x00, 0x00, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00 };
            const unsigned char fonts_10x10_2[] = { 0x00, 0x00, 0x0F, 0x09, 0x08, 0x04, 0x02, 0x01, 0x0F, 0x00 };
            const unsigned char fonts_10x10_3[] = { 0x00, 0x00, 0x0F, 0x09, 0x06, 0x08, 0x08, 0x09, 0x07, 0x00 };
            const unsigned char fonts_10x10_4[] = { 0x00, 0x00, 0x08, 0x0C, 0x0A, 0x09, 0x09, 0x0E, 0x08, 0x00 };
            const unsigned char fonts_10x10_5[] = { 0x00, 0x00, 0x0F, 0x01, 0x07, 0x08, 0x08, 0x09, 0x07, 0x00 };
            const unsigned char fonts_10x10_6[] = { 0x00, 0x00, 0x06, 0x01, 0x01, 0x0F, 0x09, 0x09, 0x06, 0x00 };
            const unsigned char fonts_10x10_7[] = { 0x00, 0x00, 0x0F, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x00 };
            const unsigned char fonts_10x10_8[] = { 0x00, 0x00, 0x0E, 0x09, 0x09, 0x06, 0x09, 0x09, 0x06, 0x00 };
            const unsigned char fonts_10x10_9[] = { 0x00, 0x00, 0x06, 0x09, 0x09, 0x0F, 0x08, 0x08, 0x06, 0x00 };
            const unsigned char fonts_10x10_10[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00 };  // 每个数组占一行
        }
        
        const FontChar fonts_10x10[] = {
            // "0"
        {
            .name = "0",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_0
        },// "1"
        {
            .name = "1",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_1
        },// "2"
        {
            .name = "2",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_2
        },// "3"
        {
            .name = "3",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_3
        },// "4"
        {
            .name = "4",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_4
        },// "5"
        {
            .name = "5",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_5
        },// "6"
        {
            .name = "6",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_6
        },// "7"
        {
            .name = "7",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_7
        },// "8"
        {
            .name = "8",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_8
        },// "9"
        {
            .name = "9",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_9
        },// "."
        {
            .name = ".",
            .width = 5,
            .height = 10,
            .data = detail::fonts_10x10_10
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
