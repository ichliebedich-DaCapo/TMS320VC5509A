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
#include <cstdio>
// =====================需要的外部函数=====================
#ifdef SIMULATOR
extern "C" {
extern void lcd_write_page(uint16_t page, const uint16_t *buf);
}
#endif

// ====================宏定义=====================
// 控制宏
#define GUI_PAGE_MODE  8    // 页模式，有8和16两种模式。两种模式均为逻辑页，实际页固定为8页。默认8页，如果要改成16页，一些函数是需要完善的

// 属性声明
#define GUI_HOR 128 // 2^7，考虑到一行正好是2^7，那么就可以通过移位代替乘法，提高效率
#define GUI_HOR_MAX_INDEX 127
#define GUI_VOR 64
#define GUI_VOR_MAX_INDEX 63
#if GUI_PAGE_MODE == 8
#define GUI_PAGE 8     // 根据具体情况，把绘制区域分成16页，左8页，右8页
#define GUI_HALF_PAGE 4
#define DIRTY_FULL_COL 0x7F00    // 全满脏列，用于重绘 8页：0x7F00 16页：0x3F00
#else
#define GUI_PAGE 16     // 根据具体情况，把绘制区域分成16页，左8页，右8页
#define GUI_HALF_PAGE 8
#define DIRTY_FULL_COL 0x3F00    // 全满脏列，用于重绘 8页：0x7F00 16页：0x3F00
#endif
#define color_white 0
#define color_black 1
#define DIRTY_MIN_COL 0x00FF    // 低8位作为最小列存储
#define DIRTY_MAX_COL 0xFF00    // 高8位作为最大列存储
#define DIRTY_DUMMY_COL 0x00FF    // 无效脏列，用于重置脏标记



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



// =====================================组件基类=====================================
class GUI_Base
{
protected:
    // 计算实际缓冲区索引。使用引用，避免输入常量，因为这种情况不应出现
    INLINE coord_t Index(const uint16_t &page, const uint16_t &x)
    {
#if GUI_PAGE_MODE == 8
        return (page << 7) + x;// 8页的情况下
#else
        return (page <<6) + x;// 16页的情况下
#endif
    }
    INLINE uint16_t get_min_col(const uint16_t &page) { return dirty_info.col[page] & DIRTY_MIN_COL; }
    INLINE uint16_t get_max_col(const uint16_t &page) { return (dirty_info.col[page] & DIRTY_MAX_COL) >> 8; }
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

    // 根据y坐标获取page
    // 128*64分成左右两个页区，左页区有0、2、4等页，由页区有1、3、5等页。由x坐标来区分左右页
    INLINE uint16_t get_page(const uint16_t &x, const uint16_t &y) { return ((y >> 3) << 1) + (x >> 6); }
    // 取低3位，相当于y对8取余，获取在一个字节的位置
    INLINE uint16_t get_mask(const uint16_t &y) { return 1 << (y & 0x07); }

    INLINE uint16_t get_dirty(const uint16_t &page) { return dirty_info.is_dirty & (1 << page); }
    INLINE void set_dirty(const uint16_t &page)
    {
        dirty_info.is_dirty |= 1 << page;
    }

    // 重置某页脏标记，包括脏列
    INLINE void reset_dirty(const uint16_t &page)
    {
        dirty_info.is_dirty &= ~(1 << page);
        dirty_info.col[page] = DIRTY_DUMMY_COL;
    }

    // 触发重绘（把所有脏标记置为1）
    static void invalidate();

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

    static void draw_hline(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);

    static void draw_vline(uint16_t y1, uint16_t y2, uint16_t x, uint16_t color);

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
#if GUI_PAGE_MODE == 8
    template<void(*oled_write_data)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)>
    static void handler();
#else
    template<void(*oled_write_data_left)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf),
    void(*oled_write_data_right)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)
>
static void handler();
#endif
    static void clear();

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

    const uint16_t page = get_page(x, y);
    const uint16_t mask = get_mask(y);

    // 原子操作更新缓冲区
    if (data)
    {
        buffer[Index(page, x)] |= mask;
    }
    else
    {
        buffer[Index(page, x)] &= ~mask;
    }

    // 更新脏区域（使用分支避免多余判断）
    set_dirty(page);
    update_col(page, x, x);
}


/*===================================缓冲区管理===================================*/
#if GUI_PAGE_MODE == 8
template<void(*oled_write_data)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)>
void GUI_Render::handler()
{
    // 阶段1：遍历组件

    // 阶段2：硬件绘制，遍历所有页
    for (uint16_t page = 0; page < GUI_PAGE; ++page)
    {
        if (!get_dirty(page)||get_min_col(page)>get_max_col(page))continue;
        oled_write_data(page, get_min_col(page), get_max_col(page),buffer+Index(page, get_min_col(page)));
        reset_dirty(page);// 重置脏标记
    }

}
#else
template<void(*oled_write_data_left)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf),
    void(*oled_write_data_right)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)
>
void GUI_Render::handler()
{
    for (uint16_t page = 0; page < GUI_PAGE; page += 2)
    {
        if (get_dirty(page))
        {
            if (get_min_col(page) <= get_max_col(page))
            {
                // page需要右移1位，因为要转为实际的页区
                oled_write_data_left(page>>1, get_min_col(page), get_max_col(page),
                                    buffer + Index(page, get_min_col(page)));
            }
            reset_dirty(page); // 重置脏标记
        }

        if (get_dirty(page + 1))
        {
            if (get_min_col(page + 1) <= get_max_col(page + 1))
            {
                // page需要右移1位，因为要转为实际的页区
                oled_write_data_right(page>>1, get_min_col(page + 1)+64, get_max_col(page + 1)+64,
                                     buffer + Index(page + 1, get_min_col(page + 1)));
            }
            reset_dirty(page + 1); // 重置脏标记
        }
    }
}


#endif

#endif //ZQ_GUI_H
