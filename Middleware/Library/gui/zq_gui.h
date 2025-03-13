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
#define GUI_VOR_MAX_INDEX 63
#define GUI_PAGE 8

/*变量声明*/
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



/*===================================图形绘制===================================*/
void gui_draw_hline(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color);

    void gui_draw_vline(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color);
//
// void gui_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, uint8_t filled);
//
// void gui_fill_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
//
// void gui_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
//
// void gui_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color);

/*===================================缓冲区管理===================================*/
void gui_clear();

void gui_refresh_buf(); // 刷新缓冲区到LCD屏
#define gui_handler() gui_refresh_buf() // GUI处理函数

#ifdef __cplusplus
}
#endif
#endif //ZQ_GUI_H
// 做得很好！现在我定义了这些宏，用于后续兼容
// #define GUI_HOR 128
// #define GUI_HOR_MAX_INDEX 127
// #define GUI_VOR 64
// #define GUI_PAGE 8
// 并且我修改了一些API的名称，比如gui_write_pixel、gui_fill_rect、gui_draw_hline、gui_draw_vline等，
// 前面的这些绘制线段、圆的API可用，现在我需要再创建一些API来绘制可以指定长度的绘制线段、圆形等
