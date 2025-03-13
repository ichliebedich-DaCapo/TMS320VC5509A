//
// Created by fairy on 2025/3/13 12:13.
//
// 头文件
#include"zq_gui.h"

// 函数
/**
 * 绘制水平线（新参数版）
 * @param x1     起始列坐标 (0~127)
 * @param length 线段长度（至少1像素）
 * @param y      行坐标 (0~63)
 * @param color  颜色，0：熄灭，1：点亮
 */
void gui_draw_hline(uint8_t x1, uint8_t length, uint8_t y, uint8_t color)
{
    /* 参数有效性检查 */
    if (y >= GUI_VOR || length == 0) return;

    /* 计算实际结束列 */
    uint8_t x2 = x1 + length - 1;
    if (x1 > GUI_HOR_MAX_INDEX) return;
    if (x2 > GUI_HOR_MAX_INDEX) x2 = GUI_HOR_MAX_INDEX;

    /* 高效页处理 */
    const uint8_t page = y >> 3;
    const uint8_t bit_mask = 1 << (y & 0x07);
    PageDirtyInfo *p = &lcd_dirty_info[page];

    /* 批量设置位 */
    uint8_t x;
    if (color)
    {
        for (x = x1; x <= x2; ++x) lcd_buffer[page][x] |= bit_mask;
    }
    else
    {
        for (x = x1; x <= x2; ++x) lcd_buffer[page][x] &= ~bit_mask;
    }

    /* 更新脏区域 */
    p->is_dirty = 1;
    if (x1 < p->min_col) p->min_col = x1;
    if (x2 > p->max_col) p->max_col = x2;
}

/**
 * 绘制垂直线（新参数版）
 * @param y1     起始行坐标 (0~63)
 * @param length 线段长度（至少1像素）
 * @param x      列坐标 (0~127)
 * @param color  颜色，0：熄灭，1：点亮
 */
void gui_draw_vline(uint8_t y1, uint8_t length, uint8_t x, uint8_t color)
{
    /* 参数有效性检查 */
    if (x >= GUI_HOR || length == 0) return;

    /* 计算实际结束行 */
    uint8_t y2 = y1 + length - 1;
    if (y1 > GUI_VOR_MAX_INDEX) return;
    if (y2 > GUI_VOR_MAX_INDEX) y2 = GUI_VOR_MAX_INDEX;

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
        const uint8_t mask = (0xFF >> (7 - end_bit + start_bit)) << start_bit;

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
 * 绘制任意方向线段（修正版Bresenham算法）
 * @param x0 起始列坐标 (0~127)
 * @param y0 起始行坐标 (0~63)
 * @param x1 结束列坐标 (0~127)
 * @param y1 结束行坐标 (0~63)
 * @param color 颜色，0：熄灭，1：点亮
 */
void gui_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    /* 参数有效性检查 */
    if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

    /* 优化水平线处理 */
    if (y0 == y1)
    {
        const uint8_t start_x = MIN(x0, x1);
        const uint8_t length = (uint8_t) (ABS_DIFF(x0, x1) + 1);
        gui_draw_hline(start_x, length, y0, color);
        return;
    }

    /* 优化垂直线处理 */
    if (x0 == x1)
    {
        const uint8_t start_y = MIN(y0, y1);
        const uint8_t length = (uint8_t) (ABS_DIFF(y0, y1) + 1);
        gui_draw_vline(start_y, length, x0, color);
        return;
    }

    /* 标准Bresenham算法实现 */
    int16_t dx = ABS_DIFF(x1, x0);
    int16_t dy = -ABS_DIFF(y1, y0);
    int8_t sx = (x0 < x1) ? 1 : -1;
    int8_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy; // 关键修正点：误差项初始化

    while (1)
    {
        gui_write_pixel(x0, y0, color);

        /* 终点检查 */
        if (x0 == x1 && y0 == y1) break;

        int16_t e2 = err << 1;

        /* X轴方向步进 */
        if (e2 >= dy)
        {
            // dy为负数，实际判断e2 >= dy
            err += dy;
            x0 += sx;
        }

        /* Y轴方向步进 */
        if (e2 <= dx)
        {
            // dx为正数，实际判断e2 <= dx
            err += dx;
            y0 += sy;
        }
    }
}
