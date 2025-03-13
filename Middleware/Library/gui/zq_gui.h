//
// Created by fairy on 2025/3/13 10:55.
//
#ifndef ZQ_GUI_H
#define ZQ_GUI_H

#ifdef __cplusplus
extern "C" {
#endif
#ifdef SIMULATOR
#include<stdint.h>
#include <zq_conf.h>
#else
#include <lcd.h>
#endif

#define GUI_HOR 128
#define GUI_HOR_MAX_INDEX 127
#define GUI_VOR 64
#define GUI_PAGE 8

/*变量*/
// 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
extern uint8_t lcd_buffer[GUI_PAGE][GUI_HOR];

// 脏页管理（结构体提高缓存效率）
typedef struct
{
    uint8_t is_dirty;
    uint8_t min_col;
    uint8_t max_col;
} PageDirtyInfo;

extern PageDirtyInfo lcd_dirty_info[GUI_PAGE];

// 需要的外部函数
#ifdef SIMULATOR
extern void lcd_write_page(uint8_t page, const uint8_t *buf);
#endif

/**
 * 设置单点像素
 * @param x 列坐标 (0~127)
 * @param y 行坐标 (0~63)
 * @param data 像素颜色，0：熄灭，1：点亮
 */
INLINE void gui_write_pixel(const uint8_t x, const uint8_t y, const uint8_t data)
{
    if (x >= GUI_HOR || y >= GUI_VOR) return;

    const uint8_t page = y >> 3;
    const uint8_t mask = 1 << (7 - (y & 0x07));

    // 原子操作更新缓冲区
    if (data)
    {
        lcd_buffer[page][x] |= mask;
    }
    else
    {
        lcd_buffer[page][x] &= ~mask;
    }

    // 更新脏区域（使用分支避免多余判断）
    lcd_dirty_info[page].is_dirty = 1;
    if (x < lcd_dirty_info[page].min_col) lcd_dirty_info[page].min_col = x;
    if (x > lcd_dirty_info[page].max_col) lcd_dirty_info[page].max_col = x;
}

// 填充矩形区域（缓冲区版）
INLINE void gui_fill_rect(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2,
                          const uint8_t color)
{
    if (x1 > x2 || y1 > y2) return;

    const uint8_t page_start = y1 >> 3;
    const uint8_t page_end = y2 >> 3;
    uint8_t page, x;
    for (page = page_start; page <= page_end; page++)
    {
        // 计算垂直掩码
        uint8_t mask = 0xFF;
        if (page == page_start) mask &= (0xFF << (8 - (y1 & 7)));
        if (page == page_end) mask &= (0xFF >> (7 - (y2 & 7)));

        // 批量填充列
        for (x = x1; x <= x2; x++)
        {
            if (color)
            {
                lcd_buffer[page][x] |= mask;
            }
            else
            {
                lcd_buffer[page][x] &= ~mask;
            }
        }

        // 更新脏区域
        lcd_dirty_info[page].is_dirty = 1;
        if (x1 < lcd_dirty_info[page].min_col) lcd_dirty_info[page].min_col = x1;
        if (x2 > lcd_dirty_info[page].max_col) lcd_dirty_info[page].max_col = x2;
    }
}

/*===================================图形绘制===================================*/
void gui_draw_hline(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color);

void gui_draw_vline(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color);

void gui_draw_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);

void gui_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

void gui_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color);

/*===================================缓冲区管理===================================*/
void gui_clear();

void gui_refresh_buf(); // 刷新缓冲区到LCD屏
#define gui_handler() gui_refresh_buf() // GUI处理函数

#ifdef __cplusplus
}
#endif
#endif //ZQ_GUI_H
