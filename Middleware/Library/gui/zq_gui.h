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
#define color_white 0
#define color_black 1

// 辅助宏
#define ABS_DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SWAP(a, b) { uint16_t t = a; a = b; b = t; }

/*变量声明*/
// 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
extern uint16_t lcd_buffer[GUI_PAGE][GUI_HOR];

// 脏页管理（结构体提高缓存效率）
typedef struct
{
    uint16_t is_dirty;
    uint16_t min_col;
    uint16_t max_col;
} PageDirtyInfo;

extern PageDirtyInfo lcd_dirty_info[GUI_PAGE];

// 需要的外部函数
#ifdef SIMULATOR
extern void lcd_write_page(uint16_t page, const uint16_t *buf);
#endif

/**
 * 设置单点像素
 * @param x 列坐标 (0~127)
 * @param y 行坐标 (0~63)
 * @param data 像素颜色，0：熄灭，1：点亮
 */
INLINE void gui_write_pixel(const uint16_t x, const uint16_t y, const uint16_t data)
{
    if (x >= GUI_HOR || y >= GUI_VOR) return;

    const uint16_t page = y >> 3;
    const uint16_t mask = 1 << (y & 0x07);

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

void gui_draw_hline(uint16_t x1, uint16_t x2, uint16_t length, uint16_t color);

void gui_draw_vline(uint16_t y1, uint16_t y2, uint16_t length, uint16_t color);

void gui_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void gui_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

void gui_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

void gui_draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

/*===================================缓冲区管理===================================*/
void gui_clear();

void gui_refresh_buf(); // 刷新缓冲区到LCD屏
#define gui_handler() gui_refresh_buf() // GUI处理函数

#ifdef __cplusplus
}
#endif
#endif //ZQ_GUI_H

