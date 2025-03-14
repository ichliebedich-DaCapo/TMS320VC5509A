//
// Created by fairy on 2025/3/13 10:55.
//
#include "zq_gui.h"
#include <string.h>

// ========================变量定义================================
uint16_t GUI_Object::count;
uint16_t GUI_Base::buffer[GUI_PAGE][GUI_HOR];
PageDirtyInfo GUI_Base:: dirty_info[GUI_PAGE]={
    {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0},
    {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}
};
// ========================基本操作================================

void GUI_Object::clear()
{
    uint16_t page;
    for (page = 0; page < 8; ++page)
    {
        dirty_info[page].is_dirty = 1;
        dirty_info[page].min_col = 0;
        dirty_info[page].max_col = 127;
        memset(buffer[page], 0, sizeof(buffer[page]));
    }
}
// ========================绘制函数================================
/**
 * 绘制水平线（新参数版）
 * @param x1     起始列坐标 (0~127)
 * @param length 线段长度（至少1像素）
 * @param y      行坐标 (0~63)
 * @param color  颜色，0：熄灭，1：点亮
 */
void GUI_Object::draw_hline(uint16_t x1, uint16_t length, uint16_t y, uint16_t color)
{
    /* 参数有效性检查 */
    if (y >= GUI_VOR || length == 0) return;

    /* 计算实际结束列 */
    uint16_t x2 = x1 + length - 1;
    if (x1 > GUI_HOR_MAX_INDEX) return;
    if (x2 > GUI_HOR_MAX_INDEX) x2 = GUI_HOR_MAX_INDEX;

    /* 高效页处理 */
    const uint16_t page = y >> 3;
    const uint16_t bit_mask = 1 << (y & 0x07);
    PageDirtyInfo *p = &dirty_info[page];

    /* 批量设置位 */
    uint16_t x;
    if (color)
    {
        for (x = x1; x <= x2; ++x) buffer[page][x] |= bit_mask;
    }
    else
    {
        for (x = x1; x <= x2; ++x) buffer[page][x] &= ~bit_mask;
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
void GUI_Object::draw_vline(uint16_t y1, uint16_t length, uint16_t x, uint16_t color)
{
    /* 参数有效性检查 */
    if (x >= GUI_HOR || length == 0) return;

    /* 计算实际结束行 */
    uint16_t y2 = y1 + length - 1;
    if (y1 > GUI_VOR_MAX_INDEX) return;
    if (y2 > GUI_VOR_MAX_INDEX) y2 = GUI_VOR_MAX_INDEX;

    /* 计算涉及的页 */
    const uint16_t start_page = y1 >> 3;
    const uint16_t end_page = y2 >> 3;

    /* 逐页处理 */
    uint16_t page;
    for (page = start_page; page <= end_page; ++page)
    {
        /* 计算当前页的y范围 */
        const uint16_t page_base = page << 3;
        uint16_t curr_y_start = (y1 > page_base) ? y1 : page_base;
        uint16_t curr_y_end = (y2 < (page_base + 7)) ? y2 : (page_base + 7);

        /* 生成连续位掩码 */
        const uint16_t start_bit = curr_y_start - page_base;
        const uint16_t end_bit = curr_y_end - page_base;
        const uint16_t mask = (0xFF >> (7 - end_bit + start_bit)) << start_bit;

        /* 更新缓冲区 */
        if (color)
        {
            buffer[page][x] |= mask;
        }
        else
        {
            buffer[page][x] &= ~mask;
        }

        /* 更新脏区域 */
        PageDirtyInfo *p = &dirty_info[page];
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
void GUI_Object::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    /* 参数有效性检查 */
    if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

    /* 优化水平线处理 */
    if (y0 == y1)
    {
        const uint16_t start_x = MIN(x0, x1);
        const uint16_t length = (uint16_t) (ABS_DIFF(x0, x1) + 1);
        draw_hline(start_x, length, y0, color);
        return;
    }

    /* 优化垂直线处理 */
    if (x0 == x1)
    {
        const uint16_t start_y = MIN(y0, y1);
        const uint16_t length = (uint16_t) (ABS_DIFF(y0, y1) + 1);
        draw_vline(start_y, length, x0, color);
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
        write_pixel(x0, y0, color);

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



/**
 * 绘制矩形（空心）
 * @param x 左上角列坐标
 * @param y 左上角行坐标
 * @param width 宽度（≥1）
 * @param height 高度（≥1）
 * @param color 颜色
 */
void GUI_Object::draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    /* 参数检查 */
    if (x >= GUI_HOR || y >= GUI_VOR || width == 0 || height == 0) return;

    /* 计算右下角坐标 */
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;
    if (x_end >= GUI_HOR) x_end = GUI_HOR_MAX_INDEX;
    if (y_end >= GUI_VOR) y_end = GUI_VOR_MAX_INDEX;

    /* 绘制四边 */
    draw_hline(x, width, y, color);          // 上边
    draw_hline(x, width, y_end, color);       // 下边
    draw_vline(y, height, x, color);          // 左边
    draw_vline(y, height, x_end, color);      // 右边
}

/**
 * 填充矩形（实心）
 * @param x 左上角列坐标
 * @param y 左上角行坐标
 * @param width 宽度（≥1）
 * @param height 高度（≥1）
 * @param color 颜色
 */
void GUI_Object::fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    /* 参数检查 */
    if (x >= GUI_HOR || y >= GUI_VOR || width == 0 || height == 0) return;

    /* 计算实际填充范围 */
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;
    if (x_end >= GUI_HOR) x_end = GUI_HOR_MAX_INDEX;
    if (y_end >= GUI_VOR) y_end = GUI_VOR_MAX_INDEX;

    /* 计算涉及的页 */
    uint16_t start_page = y >> 3;
    uint16_t end_page = y_end >> 3;

    /* 逐页填充 */
    uint16_t page;
    for (page = start_page; page <= end_page; ++page) {
        /* 计算当前页的垂直范围 */
        uint16_t page_y_start = page << 3;
        uint16_t page_y_end = page_y_start + 7;

        /* 确定当前页内的有效y范围 */
        uint16_t curr_y_start = (y > page_y_start) ? y : page_y_start;
        uint16_t curr_y_end = (y_end < page_y_end) ? y_end : page_y_end;

        /* 生成垂直掩码 */
        uint16_t start_bit = curr_y_start - page_y_start;
        uint16_t end_bit = curr_y_end - page_y_start;
        uint16_t mask = (0xFF >> (7 - end_bit + start_bit)) << start_bit;

        /* 批量填充列 */
        uint16_t col;
        if (color) {
            for (col = x; col <= x_end; ++col) {
                buffer[page][col] |= mask;
            }
        } else {
            for (col = x; col <= x_end; ++col) {
                buffer[page][col] &= ~mask;
            }
        }

        /* 更新脏区域 */
        PageDirtyInfo *p = &dirty_info[page];
        p->is_dirty = 1;
        if (x < p->min_col) p->min_col = x;
        if (x_end > p->max_col) p->max_col = x_end;
    }
}

/**
 * 绘制圆（中点圆算法）
 * @param x0 圆心列坐标
 * @param y0 圆心行坐标
 * @param radius 半径（≥1）
 * @param color 颜色
 */
void GUI_Object::draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color)
{
    /* 参数检查 */
    if (radius == 0) return;
    int16_t f = 1 - radius;
    int16_t ddF_x = 0;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    /* 绘制八个对称点 */
    while (x <= y) {
        write_pixel(x0 + x, y0 + y, color);
        write_pixel(x0 - x, y0 + y, color);
        write_pixel(x0 + x, y0 - y, color);
        write_pixel(x0 - x, y0 - y, color);
        write_pixel(x0 + y, y0 + x, color);
        write_pixel(x0 - y, y0 + x, color);
        write_pixel(x0 + y, y0 - x, color);
        write_pixel(x0 - y, y0 - x, color);

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
    }
}

// =======================================渲染引擎=====================================

