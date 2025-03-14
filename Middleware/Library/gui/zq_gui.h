//
// Created by fairy on 2025/3/13 10:55.
//
#ifndef ZQ_GUI_H
#define ZQ_GUI_H

#ifdef SIMULATOR
#include<stdint.h>
#include <zq_conf.h>
#else
#include <oled.h>
#endif
// =====================需要的外部函数=====================
#ifdef SIMULATOR
extern "C" {
extern void lcd_write_page(uint16_t page, const uint16_t *buf);
}
#endif

// ====================宏定义=====================
// 属性声明
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

// ====================数据类型声明=====================
// 脏页管理（结构体提高缓存效率）
typedef struct
{
    uint16_t is_dirty;
    uint16_t min_col;
    uint16_t max_col;
} PageDirtyInfo;

// =====================================组件基类=====================================
class GUI_Base
{
protected:
    static uint16_t buffer[GUI_PAGE][GUI_HOR]; // 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
    static PageDirtyInfo dirty_info[GUI_PAGE];
};

class GUI_Object : public GUI_Base
{
public:
    // 基本操作
    static void create() { ++count; }
    static void destroy() { --count; }
    static void clear();

    // 基本绘制函数
    INLINE void write_pixel(const uint16_t x, const uint16_t y, const uint16_t data);

    static void draw_hline(uint16_t x1, uint16_t x2, uint16_t length, uint16_t color);

    static void draw_vline(uint16_t y1, uint16_t y2, uint16_t length, uint16_t color);

    static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    static void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

protected:
    static uint16_t count; // 组件数量
};

// =====================================渲染引擎=====================================

class GUI_Render : public GUI_Base
{
public:
#ifdef SIMULATOR
    template<void(*oled_write_page)(uint16_t page, const uint16_t *buf)>
    static void handler();
#else
    static void handler();
#endif
};


// ==================================内联函数定义=========================================
/**
 * 设置单点像素
 * @param x 列坐标 (0~127)
 * @param y 行坐标 (0~63)
 * @param data 像素颜色，0：熄灭，1：点亮
 */
void GUI_Object::write_pixel(const uint16_t x, const uint16_t y, const uint16_t data)
{
    if (x >= GUI_HOR || y >= GUI_VOR) return;

    const uint16_t page = y >> 3;
    const uint16_t mask = 1 << (y & 0x07);

    // 原子操作更新缓冲区
    if (data)
    {
        buffer[page][x] |= mask;
    }
    else
    {
        buffer[page][x] &= ~mask;
    }

    // 更新脏区域（使用分支避免多余判断）
    dirty_info[page].is_dirty = 1;
    if (x < dirty_info[page].min_col) dirty_info[page].min_col = x;
    if (x > dirty_info[page].max_col) dirty_info[page].max_col = x;
}


/*===================================缓冲区管理===================================*/

#ifdef SIMULATOR
template<void(*oled_write_page)(uint16_t page, const uint16_t *buf)>
void GUI_Render::handler()
{
    uint16_t page;
    for (page = 0; page < 8; page++)
    {
        PageDirtyInfo *p = &dirty_info[page];
        if (!p->is_dirty || p->min_col > p->max_col) continue;

        oled_write_page(page,buffer[page]);

        // 重置脏标记
        p->is_dirty = 0;
        p->min_col = GUI_HOR;
        p->max_col = 0;
    }
}

#endif


#endif //ZQ_GUI_H
