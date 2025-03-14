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
#define GUI_HOR 128 // 2^7，考虑到一行正好是2^7，那么就可以通过移位代替乘法，提高效率
#define GUI_HOR_MAX_INDEX 127
#define GUI_VOR 64
#define GUI_VOR_MAX_INDEX 63
#define GUI_PAGE 16     // 根据具体情况，把绘制区域分成16页，左8页，右8页
#define color_white 0
#define color_black 1
#define DIRTY_MIN_COL 0x00FF    // 低8位作为最小列存储
#define DIRTY_MAX_COL 0xFF00    // 高8位作为最大列存储
#define DIRTY_DEFAULT_COL 0x00FF    // 默认脏列，用于重置脏标记

// ====================数据类型声明=====================
// 类型别名
typedef uint16_t coord_t; // 坐标类型

// 脏页管理（结构体提高缓存效率）
typedef struct
{
    uint16_t is_dirty; // 考虑到共16页，那么每位可作为一页的标志
    uint16_t col[GUI_PAGE]; // 考虑到1页只有128列，用8位即可存储，那么一个16位即可存储最小列和最大列
} PageDirtyInfo;


// 辅助内联函数
INLINE void swap(uint16_t &a, uint16_t &b) noexcept
{
    const uint16_t t = a;
    a = b;
    b = t;
}

INLINE uint16_t min(const uint16_t &a, const uint16_t &b)
{
    return a < b ? a : b;
}

INLINE uint16_t max(const uint16_t &a, const uint16_t &b)
{
    return a > b ? a : b;
}

// 绝对值差
INLINE uint16_t abs_diff(const uint16_t &a, const uint16_t &b)
{
    return ((a) > (b) ? (a) - (b) : (b) - (a));
}

// 计算实际缓冲区索引。使用引用，避免输入常量，因为这种情况不应出现
INLINE coord_t Coord(const uint16_t &page, const uint16_t &col)
{
    return (page << 7) + col;
}

// =====================================组件基类=====================================
class GUI_Base
{
protected:
    INLINE uint16_t get_min_col(const uint16_t &page) { return dirty_info.col[page] & DIRTY_MIN_COL; }
    INLINE uint16_t get_max_col(const uint16_t &page) { return dirty_info.col[page] & DIRTY_MAX_COL; }
    INLINE void set_min_col(const uint16_t &page, const uint16_t &col)
    {
        dirty_info.col[page] &= ~DIRTY_MIN_COL;
        dirty_info.col[page] |= col;
    }

    INLINE void set_max_col(const uint16_t &page, const uint16_t &col)
    {
        dirty_info.col[page] &= ~DIRTY_MAX_COL;
        dirty_info.col[page] |= col << 8;
    }

    INLINE void set_col(const uint16_t &page, const uint16_t &min_col, const uint16_t &max_col)
    {
        dirty_info.col[page] = (min_col & DIRTY_MIN_COL) | ((max_col << 8) & DIRTY_MAX_COL);
    }

    // 更新脏列。如果最小列比旧值小，那么更新最小列；如果最大列比旧值大，那么更新最大列
    INLINE void update_col(const uint16_t &page, const uint16_t &min_col, const uint16_t &max_col)
    {
        if (min_col < get_min_col(page)) set_min_col(page, min_col);
        if (max_col > get_max_col(page)) set_max_col(page, max_col);
    }

    // 设置脏标记并更新脏列（如果最小列比旧值小，那么更新最小列；如果最大列比旧值大，那么更新最大列）
    INLINE void set_dirty_update_col(const uint16_t &page, const uint16_t &min_col, const uint16_t &max_col)
    {
        dirty_info.is_dirty |= 1 << page;
        update_col(page, min_col, max_col);
    }

    INLINE uint16_t get_dirty(const uint16_t &page) { return dirty_info.is_dirty & (1 << page); }
    INLINE void set_dirty(const uint16_t &page)
    {
        dirty_info.is_dirty |= 1 << page;
    }

    // 重置脏标记，包括脏列
    INLINE void reset_dirty(const uint16_t &page)
    {
        dirty_info.is_dirty &= ~(1 << page);
        dirty_info.col[page] = DIRTY_DEFAULT_COL;
    }

protected:
    static uint16_t buffer[GUI_PAGE * GUI_HOR]; // 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
    static PageDirtyInfo dirty_info;
};

/**
 * 基本组件
 * @note
 */
class GUI_Object : public GUI_Base
{
public:
    // 基本操作
    static void create() { ++count; }
    static void destroy() { --count; }


    // 基本绘制函数
    INLINE void write_pixel(uint16_t x, uint16_t y, uint16_t data);

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

    static void clear();
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
        buffer[Coord(page, x)] |= mask;
    }
    else
    {
        buffer[Coord(page, x)] &= ~mask;
    }

    // 更新脏区域（使用分支避免多余判断）
    set_dirty(page);
    update_col(page, x, x);
}


/*===================================缓冲区管理===================================*/
#ifdef SIMULATOR
template<void(*oled_write_page)(uint16_t page, const uint16_t *buf)>
void GUI_Render::handler()
{
    for (uint16_t page = 0; page < 8; page++)
    {
        if (!get_dirty(page) || get_min_col(page) > get_max_col(page)) continue;

        oled_write_page(page, &buffer[page]);

        // 重置脏标记
        reset_dirty(page);
    }
}

#endif


#endif //ZQ_GUI_H
