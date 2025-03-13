//
// Created by fairy on 2025/3/13 10:55.
//
#ifndef ZQ_GUI_H
#define ZQ_GUI_H

#ifdef __cplusplus
extern "C" {
#endif
#include <lcd.h>

/*变量*/
// 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
extern uint8_t lcd_buffer[8][128];
// 标记页是否被修改，减少刷新范围
extern uint8_t lcd_page_dirty[8];

// 需要的外部函数
#ifdef SIMULATOR
extern void lcd_write_page(uint8_t page,uint8_t*buf);
#endif

/**
 * 设置单点像素
 * @param x 列坐标 (0~127)
 * @param y 行坐标 (0~63)
 * @param data 像素颜色，0：熄灭，1：点亮
 */
INLINE void gui_write_pixel_buf(const uint8_t x, const uint8_t y, const uint8_t data)
{
    // 计算页和位位置（无除法/取余）
    const uint8_t page = y >> 3; // y / 8 → y >> 3
    const uint8_t bit_pos = 7 - (y & 0x7); // y % 8 → y & 0x7

    // 修改缓冲区
    if (data)
    {
        lcd_buffer[page][x] |= (1 << bit_pos);
    }
    else
    {
        lcd_buffer[page][x] &= ~(1 << bit_pos);
    }

    // 标记页为脏
    lcd_page_dirty[page] = 1;
}

// 填充矩形区域（缓冲区版）
INLINE void gui_fill_rect_buf(const uint8_t x_start, const uint8_t x_end, const uint8_t y_start, const uint8_t y_end)
{
    uint8_t x, y;
    for (y = y_start; y <= y_end; ++y)
    {
        const uint8_t page = y >> 3;
        const uint8_t bit_mask = 1 << (7 - (y & 0x7));
        for (x = x_start; x <= x_end; ++x)
        {
            lcd_buffer[page][x] |= bit_mask;
        }
        lcd_page_dirty[page] = 1;
    }
}

void gui_refresh_buf(); // 刷新缓冲区到LCD屏
#define gui_handler() gui_refresh_buf() // GUI处理函数

#ifdef __cplusplus
}
#endif
#endif //ZQ_GUI_H
