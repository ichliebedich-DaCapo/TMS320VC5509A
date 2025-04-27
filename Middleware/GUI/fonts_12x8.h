#ifndef ZQ_FONT_6X8_H
#define ZQ_FONT_6X8_H
#include<zq_font_base.h>

namespace GUI
{
    namespace Font
    {
        // 所有字模数据数组定义
        namespace detail
        {
            const unsigned char fonts_12x8_0[] = { 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x3F, 0x00 };
            const unsigned char fonts_12x8_1[] = { 0x00, 0x00, 0x1E, 0x01, 0x01, 0x11, 0x0E, 0x00 };
            const unsigned char fonts_12x8_2[] = { 0x00, 0x00, 0x0F, 0x12, 0x12, 0x12, 0x0F, 0x00 };
            const unsigned char fonts_12x8_3[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            const unsigned char fonts_12x8_4[] = { 0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00 };
            const unsigned char fonts_12x8_5[] = { 0x00, 0x00, 0x3B, 0x16, 0x16, 0x1A, 0x17, 0x00 };
            const unsigned char fonts_12x8_6[] = { 0x00, 0x00, 0x1F, 0x0E, 0x0A, 0x02, 0x1F, 0x00 };  // 每个数组占一行
        }
        
        const FontChar fonts_12x8[] = {
            // "L"
        {
            .name = "L",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_0
        },// "C"
        {
            .name = "C",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_1
        },// "D"
        {
            .name = "D",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_2
        },// " "
        {
            .name = " ",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_3
        },// "O"
        {
            .name = "O",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_4
        },// "N"
        {
            .name = "N",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_5
        },// "E"
        {
            .name = "E",
            .width = 6,
            .height = 8,
            .data = detail::fonts_12x8_6
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
