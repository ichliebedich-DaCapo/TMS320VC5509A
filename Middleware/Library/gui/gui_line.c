//
// Created by fairy on 2025/3/13 12:13.
//
// 头文件
#include"zq_gui.h"

// 函数
/* 水平线绘制 */
void gui_draw_hline(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color)
{
    /* 参数有效性检查 */
    if (y >= GUI_VOR) return;
    if (x1 > x2)
    {
        const uint8_t t = x1;
        x1 = x2;
        x2 = t;
    }
    if (x2 >= GUI_HOR) x2 = GUI_HOR_MAX_INDEX;

    /* 高效页处理 */
    const uint8_t page = y >> 3;
    const uint8_t bit_mask = 1 <<( y & 0x07);
    PageDirtyInfo *p = &lcd_dirty_info[page];

    uint8_t x;
    /* 批量设置位 */
    if (color)
    {
        for (x = x1; x <= x2; ++x)
        {
            lcd_buffer[page][x] |= bit_mask;
        }
    }
    else
    {
        for (x = x1; x <= x2; ++x)
        {
            lcd_buffer[page][x] &= ~bit_mask;
        }
    }

    /* 更新脏区域 */
    p->is_dirty = 1;
    if (x1 < p->min_col) p->min_col = x1;
    if (x2 > p->max_col) p->max_col = x2;
}

/**
 * 绘制垂直线（修正版）
 * @param x    列坐标 (0~127)
 * @param y1   起始行坐标 (0~63)
 * @param y2   结束行坐标 (0~63)
 * @param color 颜色，0：熄灭，1：点亮
 */
void gui_draw_vline(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color)
{
    /* 参数有效性检查 */
    if (x >= GUI_HOR) return;
    if (y1 > y2)
    {
        const uint8_t t = y1;
        y1 = y2;
        y2 = t;
    }
    if (y1 >= GUI_VOR) return;
    if (y2 >= GUI_VOR) y2 = GUI_VOR_MAX_INDEX;

    /* 计算涉及的页 */
    const uint8_t start_page = y1 >> 3;
    const uint8_t end_page = y2 >> 3;

    /* 逐页处理 */
    uint8_t page;
    for (page = start_page; page <= end_page; ++page)
    {
        /* 计算当前页的y范围 */
        const uint8_t page_base = page << 3;
        uint8_t curr_y_start = (y1 > page_base) ? y1 : page_base;
        uint8_t curr_y_end = (y2 < (page_base + 7)) ? y2 : (page_base + 7);

        /* 生成连续位掩码 */
        const uint8_t start_bit = curr_y_start - page_base;
        const uint8_t end_bit = curr_y_end - page_base;
        const uint8_t bit_count = end_bit - start_bit + 1;
        const uint8_t mask = (0xFF >> (8 - bit_count)) << start_bit;

        /* 更新缓冲区 */
        if (color)
        {
            lcd_buffer[page][x] |= mask;
        }
        else
        {
            lcd_buffer[page][x] &= ~mask;
        }

        /* 更新脏区域 */
        PageDirtyInfo *p = &lcd_dirty_info[page];
        p->is_dirty = 1;
        if (x < p->min_col) p->min_col = x;
        if (x > p->max_col) p->max_col = x;
    }
}
/**
 * 绘制任意方向线段（Bresenham算法实现）
 * @param x0 起始点列坐标
 * @param y0 起始点行坐标
 * @param x1 结束点列坐标
 * @param y1 结束点行坐标
 * @param color 颜色，0：熄灭，1：点亮
 */
void gui_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    /* 参数有效性检查 */
    if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

    /* Bresenham算法参数 */
    int16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int16_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int8_t sx = (x0 < x1) ? 1 : -1;
    int8_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t e2;

    /* 主绘制循环 */
    while (1)
    {
        gui_write_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        e2 = err << 1;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}