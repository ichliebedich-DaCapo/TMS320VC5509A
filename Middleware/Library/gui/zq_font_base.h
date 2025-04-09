//
// Created by fairy on 2025/4/9 15:20.
//
#ifndef ZQ_FONT_BASE_H
#define ZQ_FONT_BASE_H
#include <cstring>
#include<zq_conf.h>

namespace GUI
{
    // 行列式，逆向
    namespace Font
    {
        // 字模结构体（包含字符名称、宽度、高度和字模数据）
        typedef struct
        {
            const char *name; // 字符名称（如"8"、"中"）
            int width; // 字符宽度（像素）
            int height; // 字符高度（像素）
            const uint16_t *data; // 字模数据指针
        } FontChar;

        // 根据名称查找字符
        inline const FontChar *find_char_by_name(const FontChar font_table[], const uint16_t size,const char *name)
        {
            for (uint16_t i = 0; i < size; ++i)
            {
                if (strcmp(font_table[i].name, name) == 0)
                {
                    return &font_table[i];
                }
            }
            return nullptr; // 未找到
        }

#define FIND_CHAR(font_table, name) \
    find_char_by_name(font_table, sizeof(font_table)/sizeof(FontChar), name)

}
}
#endif //ZQ_FONT_BASE_H