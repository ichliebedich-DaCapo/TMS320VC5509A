//
// Created by fairy on 2025/3/13 10:55.
//
#include "zq_gui.h"
#ifndef SIMULATOR
#include <oled.h>
#endif

namespace GUI
{
    // 变量定义
    unsigned char Base::buffer[GUI_PAGE][GUI_HOR];

    uint16_t FlagBase::flag = 0;
}


// =====================函数定义==========================
namespace GUI
{
    /**
    * @brief 绘制水平线段（默认黑色）
    * @param x_start 起始点横坐标
    * @param x_end 结束点横坐标
    * @param y 固定纵坐标
    */
    void Tools::draw_hline(uint16_t x_start, uint16_t x_end, const uint16_t y)
    {
        if (y >= GUI_VOR) return; // 越界检查
        if (x_start > x_end) swap(x_start, x_end); // 确保顺序

        // 限制坐标在屏幕范围内
        x_start = (x_start >= GUI_HOR) ? GUI_HOR - 1 : x_start;
        x_end = (x_end >= GUI_HOR) ? GUI_HOR - 1 : x_end;

        const uint16_t page = y >> 3; // 计算所在页
        const uint16_t mask = 1 << (y & 0x07); // 生成位掩码

        // 批量设置该行所有列的对应位
        for (uint16_t x = x_start; x <= x_end; ++x)
        {
            buffer[page][x] |= mask;
        }
    }


    /**
    * @brief 绘制垂直线段（默认黑色）
    * @param x 固定横坐标
    * @param y_start 起始点纵坐标
    * @param y_end 结束点纵坐标
    */
    void Tools::draw_vline(const uint16_t x, uint16_t y_start, uint16_t y_end)
    {
        if (x >= GUI_HOR) return; // 越界检查
        if (y_start > y_end) swap(y_start, y_end); // 确保顺序

        // 限制坐标在屏幕范围内
        y_start = (y_start >= GUI_VOR) ? GUI_VOR - 1 : y_start;
        y_end = (y_end >= GUI_VOR) ? GUI_VOR - 1 : y_end;

        const uint16_t start_page = y_start >> 3; // 起始页
        const uint16_t end_page = y_end >> 3; // 结束页

        // 逐页设置位掩码
        for (uint16_t page = start_page; page <= end_page; ++page)
        {
            uint16_t bit_mask = 0xFF; // 默认全页覆盖
            if (page == start_page)
            {
                // 起始页的特殊处理
                bit_mask &= (0xFF << (y_start & 0x07));
            }
            if (page == end_page)
            {
                // 结束页的特殊处理
                bit_mask &= (0xFF >> (7 - (y_end & 0x07)));
            }
            buffer[page][x] |= bit_mask; // 应用位掩码
        }
    }


    /**
       * @brief 使用Bresenham算法绘制直线（默认黑色）
       * @param x0 起点横坐标
       * @param y0 起点纵坐标
       * @param x1 终点横坐标
       * @param y1 终点纵坐标
       */
    void Tools::draw_line(uint16_t x0, uint16_t y0, const uint16_t x1, const uint16_t y1)
    {
        // 坐标越界直接返回
        if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

        const int16_t dx = abs_diff(x0, x1);
        const int16_t dy = -abs_diff(y0, y1);
        const int16_t sx = (x0 < x1) ? 1 : -1;
        const int16_t sy = (y0 < y1) ? 1 : -1;
        int16_t err = dx + dy;

        while (true)
        {
            write_pixel(x0, y0); // 调用默认黑色像素绘制
            if (x0 == x1 && y0 == y1) break;
            const int16_t e2 = err << 1;
            if (e2 >= dy)
            {
                // 水平步进
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                // 垂直步进
                err += dx;
                y0 += sy;
            }
        }
    }

    void Tools::draw_char(const char *name, const uint16_t x, const uint16_t y, const Font::FontChar fonts[])
    {
        const Font::FontChar *fc = Font::find_char_by_name(fonts, name);
        if (!fc) return;

        uint16_t total_bits = fc->width;
        const unsigned char *font_data = fc->data;
        const uint16_t char_height = fc->height;
        const uint16_t char_length = fc->width / fc->height + 1;

        for (uint16_t length = 0; length < char_length; ++length)
        {
            const uint16_t start_index = length * char_height; // 字符数据起始位置
            const uint16_t start_x = x + (length << 3); // 字符数据起始位置 因为一个字节是8位，所以需要左移3位
            const uint16_t bits = (total_bits >> 3) ? 8 : total_bits; // 获取当前行需要绘制的位数

            for (uint16_t row_char = 0; row_char < char_height; ++row_char)
            {
                const unsigned char byte = font_data[start_index + row_char];
                for (uint16_t bit = 0; bit < bits; ++bit)
                {
                    if (byte & (1 << bit))
                    {
                        const uint16_t y_total = y + row_char;
                        const uint16_t x_total = start_x + bit;

                        if (y_total < 0 || y_total >= GUI_VOR) continue;
                        if (x_total < 0 || x_total >= GUI_HOR) continue;

                        const uint16_t page = y_total >> 3;
                        const uint16_t row_in_page = y_total & 0x07;
                        buffer[page][x_total] |= (1 << row_in_page);
                    }
                }
            }

            total_bits -= bits; // 减去绘制位数
        }
    }

    // 绘制字符串
    void Tools::draw_string(const char *str, const uint16_t x, const uint16_t y, const Font::FontChar fonts[])
    {
        uint16_t x_offset = 0;
        while (*str)
        {
            uint16_t len = 0;
            // 确定当前字符的字节长度
            if ((*str & 0x80) == 0)
            {
                len = 1;
            }
            else if ((*str & 0xE0) == 0xC0)
            {
                len = 2;
            }
            else if ((*str & 0xF0) == 0xE0)
            {
                len = 3;
            }
            else if ((*str & 0xF8) == 0xF0)
            {
                len = 4;
            }
            else
            {
                // 处理非法字节，这里假设跳过1个字节
                len = 1;
            }

            // 使用临时数组存储当前字符的字节
            char temp[5]; // 最大4字节 + 终止符
            std::memcpy(temp, str, len);
            temp[len] = '\0';

            // 在字模表中查找匹配项
            const Font::FontChar *fc = Font::find_char_by_name(fonts, temp);

            // 继续处理下一个字符
            str += len;

            uint16_t total_bits = fc->width;
            const unsigned char *font_data = fc->data;
            const uint16_t char_height = fc->height;
            const uint16_t char_length = fc->width / fc->height + 1;

            for (uint16_t length = 0; length < char_length; ++length)
            {
                const uint16_t start_index = length * char_height; // 字符数据起始位置
                const uint16_t start_x = x + x_offset + (length << 3); // 字符数据起始位置 因为一个字节是8位，所以需要左移3位
                const uint16_t bits = (total_bits >> 3) ? 8 : total_bits; // 获取当前行需要绘制的位数

                for (uint16_t row_char = 0; row_char < char_height; ++row_char)
                {
                    const unsigned char byte = font_data[start_index + row_char];
                    for (uint16_t bit = 0; bit < bits; ++bit)
                    {
                        if (byte & (1 << bit))
                        {
                            const uint16_t y_total = y + row_char;
                            const uint16_t x_total = start_x + bit;

                            if (y_total < 0 || y_total >= GUI_VOR) continue;
                            if (x_total < 0 || x_total >= GUI_HOR) continue;

                            const uint16_t page = y_total >> 3;
                            const uint16_t row_in_page = y_total & 0x07;
                            buffer[page][x_total] |= (1 << row_in_page);
                        }
                    }
                }

                total_bits -= bits; // 减去绘制位数

            }
            x_offset += fc->width; // 绘制完一个字符后，偏移x轴
        }
    }
}
