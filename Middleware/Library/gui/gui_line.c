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
    if(y >= GUI_VOR) return;
    if(x1 > x2) { const uint8_t t = x1; x1 = x2; x2 = t; }
    if(x2 >= GUI_HOR) x2 = GUI_HOR_MAX_INDEX;

    /* 高效页处理 */
    const uint8_t page = y >> 3;
    const uint8_t bit_mask = 1 << (7 - (y & 0x07));
    PageDirtyInfo* p = &lcd_dirty_info[page];

    uint8_t x;
    /* 批量设置位 */
    if(color) {
        for( x = x1; x <= x2; ++x) {
            lcd_buffer[page][x] |= bit_mask;
        }
    } else {
        for( x = x1; x <= x2; ++x) {
            lcd_buffer[page][x] &= ~bit_mask;
        }
    }

    /* 更新脏区域 */
    p->is_dirty = 1;
    if(x1 < p->min_col) p->min_col = x1;
    if(x2 > p->max_col) p->max_col = x2;
}

/* 垂直线绘制（优化多页处理） */
void gui_draw_vline(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color)
{
    /* 参数规范化 */
    if(x >= GUI_HOR) return;
    if(y1 > y2) { uint8_t t = y1; y1 = y2; y2 = t; }
    if(y2 >= GUI_VOR) y2 = 63;

    /* 分页处理 */
    const uint8_t page_start = y1 >> 3;
    const uint8_t page_end = y2 >> 3;
    uint8_t page;

    for( page = page_start; page <= page_end; ++page) {
        /* 计算垂直掩码 */
        uint8_t mask = 0xFF;
        const uint8_t y_min = (page == page_start) ? y1 : (page << 3);
        const uint8_t y_max = (page == page_end)   ? y2 : ((page << 3) + 7);

        mask &= (0xFF << (8 - (y_min & 0x7)));
        mask &= (0xFF >> (7 - (y_max & 0x7)));

        /* 更新缓冲区和脏区域 */
        PageDirtyInfo* p = &lcd_dirty_info[page];
        if(color) {
            lcd_buffer[page][x] |= mask;
        } else {
            lcd_buffer[page][x] &= ~mask;
        }
        p->is_dirty = 1;
        if(x < p->min_col) p->min_col = x;
        if(x > p->max_col) p->max_col = x;
    }
}

/* 矩形框绘制（优化边角处理） */
void gui_draw_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    /* 参数规范化 */
    if(x1 > x2) { uint8_t t = x1; x1 = x2; x2 = t; }
    if(y1 > y2) { uint8_t t = y1; y1 = y2; y2 = t; }
    if(x2 >= GUI_HOR) x2 = GUI_HOR_MAX_INDEX;
    if(y2 >= GUI_VOR) y2 = 63;

    /* 绘制四边 */
    gui_draw_hline(x1, x2, y1, color);  // 上边
    gui_draw_hline(x1, x2, y2, color);  // 下边
    gui_draw_vline(x1, y1+1, y2-1, color); // 左边
    gui_draw_vline(x2, y1+1, y2-1, color); // 右边
}

/* Bresenham直线算法（全方向支持） */
void gui_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    /* 参数声明 */
    int dx, dy, sx, sy, err, e2;

    /* 参数计算 */
    dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;

    /* 主绘制循环 */
    while(1) {
        gui_write_pixel(x0, y0, color);
        if(x0 == x1 && y0 == y1) break;

        e2 = err << 1;
        if(e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if(e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/* 中点圆算法（八对称优化） */
void gui_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color)
{
    /* 参数声明 */
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    /* 绘制八个对称点 */
    while(x <= y) {
        gui_write_pixel(x0 + x, y0 + y, color);
        gui_write_pixel(x0 - x, y0 + y, color);
        gui_write_pixel(x0 + x, y0 - y, color);
        gui_write_pixel(x0 - x, y0 - y, color);
        gui_write_pixel(x0 + y, y0 + x, color);
        gui_write_pixel(x0 - y, y0 + x, color);
        gui_write_pixel(x0 + y, y0 - x, color);
        gui_write_pixel(x0 - y, y0 - x, color);

        if(f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
    }
}