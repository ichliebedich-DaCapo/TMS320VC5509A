//
// Created by fairy on 2025/4/9 13:04.
//
#ifndef ZQ_FONT_H
#define ZQ_FONT_H
#include<zq_font_base.h>

namespace GUI
{
    namespace Font
    {

        const FontChar font_12x12[] = {
            // 数字"0"
            {
                .name = "0",
                .width = 6,
                .height = 12,
                .data = (const uint16_t[]){
                    0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00
                }
            },
            // 数字"1"
            {
                .name = "1",
                .width = 6,
                .height = 12,
                .data = (const uint16_t[]){
                    0x00,0x00,0x04,0x06,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00
                }
            },
            // "家"
            {
                .name = "家",
                .width = 12,
                .height = 12,
                .data = (const uint16_t[]){
                    0x20, 0xFF, 0x01, 0xFE, 0x10, 0x28, 0x56, 0xC8, 0x66, 0x58, 0x47, 0x30, 0x00, 0x07, 0x04, 0x03,
                    0x00, 0x02, 0x01, 0x00, 0x01, 0x06, 0x00, 0x00
                }
            },
        };





        // 字模表大小（字符数量）



    }
}


#endif //ZQ_FONT_H
