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
#include <cstring>
// =====================需要的外部函数=====================


// ====================宏定义=====================
// 控制宏
#define GUI_PAGE_MODE  8    // 页模式，有8和16两种模式。两种模式均为逻辑页，实际页固定为8页。默认8页，如果要改成16页，一些函数是需要完善的
#define GUI_MAX_OBJ_NUM 16 // 设置最大组件数量，静态分配内存

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

// 辅助宏
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define COMPILE_TIME_ASSERT(pred) \
typedef char CONCATENATE(static_assertion_, __COUNTER__)[(pred) ? 1 : -1]
// // 如果编译器不支持 __COUNTER__，可以使用 __LINE__
// #define COMPILE_TIME_ASSERT_LINE(pred, msg) \
// typedef char CONCATENATE(static_assertion_, __LINE__)[(pred) ? 1 : -1]

// 根据高度，获得所需的页数，用于定义数组
#define GUI_PAGE_HEIGHT(height) (((height)>>3)+((height)&0x7?1:0))

// ====================数据类型声明=====================
// 类型别名
typedef uint16_t coord_t; // 坐标类型
typedef void (*DrawFunc)(); // 定义函数指针类型

// 脏页管理（结构体提高缓存效率）
typedef struct
{
    uint16_t is_dirty; // 考虑到共16页，那么每位可作为一页的标志
    uint16_t col[GUI_PAGE]; // 考虑到1页只有128列，用8位即可存储，那么一个16位即可存储最小列和最大列
} PageDirtyInfo;



// =====================辅助内联函数====================
INLINE void swap(uint16_t &a, uint16_t &b)
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
    INLINE coord_t Index_xy(const uint16_t &x, uint16_t y)
    {
#if GUI_PAGE_MODE == 8
        y = get_page(y);// 获取页数
        return (y << 7) + x;// 8页的情况下
#else
        y = get_page(x, y);// 获取页数
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

    // 设置脏标记并更新脏列（如果最小列比旧值小，那么更新最小列；如果最大列比旧值大，那么更新最大列）
    INLINE void update_col(const uint16_t &page, const uint16_t &min_col, const uint16_t &max_col)
    {
        dirty_info.is_dirty |= 1 << page;
        if (min_col < get_min_col(page)) set_min_col(page, min_col);
        if (max_col > get_max_col(page)) set_max_col(page, max_col);
    }

    // 根据y坐标获取page
    // 128*64分成左右两个页区，左页区有0、2、4等页，由页区有1、3、5等页。由x坐标来区分左右页
#if GUI_PAGE_MODE == 8
    INLINE uint16_t get_page( const uint16_t &y)
    {
        return (y >> 3) ;
    }
#else
    INLINE uint16_t get_page(const uint16_t &x, const uint16_t &y)
    {

        return ((y >> 3) << 1) + (x >> 6);
    }
#endif

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

    /**
    * 缓冲区拷贝，把缓冲区拷贝到显示缓冲区中
    * @tparam x 显示缓冲区x坐标
    * @tparam y 显示缓冲区y坐标
    * @tparam buf 缓冲区指针，即需要拷贝的缓冲区指针
    * @tparam width 缓冲区宽度
    * @tparam height 缓冲区高度
    */
    template<uint16_t x,uint16_t y,uint16_t*buf, uint16_t width, uint16_t height>
    static void buffer_copy()
    {
        for (uint16_t i=0;i<GUI_PAGE_HEIGHT(height);++i)
        {
            memcpy(buffer+Index_xy(x,y)+GUI_HOR*i,buf+i*width,width*sizeof(uint16_t));
        }
    }

protected:
    static uint16_t buffer[GUI_PAGE * GUI_HOR]; // 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
    static DrawFunc obj_list[GUI_MAX_OBJ_NUM];
    static PageDirtyInfo dirty_info;
    static uint16_t count; // 组件数量
};

/**
 * 基本组件
 * @note
 */
class GUI_Object : public GUI_Base
{
public:
    // 基本操作
    static void create(const DrawFunc& draw) { obj_list[count] = draw;++count; }// 没有进行判断是否越界，因为目前的界面非常简单，暂时没必要
    static void destroy() { --count; }


    // =======基本绘制函数======
    INLINE void write_pixel(uint16_t x, uint16_t y, uint16_t data);

    static void draw_hline(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);

    static void draw_vline(uint16_t y1, uint16_t y2, uint16_t x, uint16_t color);

    static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    static void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

    // 模板化 默认黑色
    template<coord_t x1,coord_t x2,coord_t y,uint16_t* buf,uint16_t width,uint16_t height>
    static void draw_hline();

    template < coord_t y1, coord_t y2,coord_t x,uint16_t* buf, coord_t width, coord_t height>
    static void draw_vline();

protected:


};
// =====================================示波器组件=====================================



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

#if GUI_PAGE_MODE == 8
    const uint16_t page = get_page(y);
#else
    const uint16_t page = get_page(x, y);
#endif

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

    // // 更新脏区域（使用分支避免多余判断）
    // set_dirty(page);
    // update_col(page, x, x);
}


/*===================================缓冲区管理===================================*/
#if GUI_PAGE_MODE == 8
template<void(*oled_write_data)(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)>
void GUI_Render::handler()
{
    // 阶段1：遍历组件
    for (uint16_t i = 0; i < count; ++i)
    {
        obj_list[i]();
    }

    // 阶段2：硬件绘制

    /*一次遍历所有页，阻塞时间长，不易造成画面撕裂*/
    for (uint16_t page = 0; page < GUI_PAGE; ++page)
    {
        if (!get_dirty(page))continue;
        oled_write_data(page, get_min_col(page), get_max_col(page),buffer+Index(page, get_min_col(page)));
        reset_dirty(page);// 重置脏标记
    }

    /*一次只刷新一页，阻塞时间短，可能会造成画面撕裂*/
    // static uint16_t page = 0;
    // if (get_dirty(page))
    // {
    //     oled_write_data(page, get_min_col(page), get_max_col(page),buffer+Index(page, get_min_col(page)));
    //     reset_dirty(page);// 重置脏标记
    // }
    // page = (++page)&0x07;

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

// ====================================模板函数实现====================================

template <coord_t x1, coord_t x2, coord_t y,uint16_t* buf, coord_t width, coord_t height>
void GUI_Object::draw_hline()
{
    // 编译期坐标安全检查（C++98兼容方案）
    COMPILE_TIME_ASSERT(y < height);
    COMPILE_TIME_ASSERT(x1 < width);
    COMPILE_TIME_ASSERT(x2 < width);
    COMPILE_TIME_ASSERT(x1 <= x2);

    // 计算目标存储页（每页8行）
    const coord_t page = get_page(y);  // 等价于 y / 8

    // 生成目标位的掩码（黑色对应置1）
    const uint16_t bit_mask = get_mask(y);

    // 线性缓冲区访问模式
    const coord_t start_index= page * width;
    for(coord_t x = x1; x <= x2; ++x)
    {
        // 计算目标存储单元在缓冲区中的位置
        buf[start_index + x] |= bit_mask;
    }
}

template < coord_t y1, coord_t y2,coord_t x,uint16_t* buf, coord_t width, coord_t height>
void GUI_Object::draw_vline()
{
    COMPILE_TIME_ASSERT(x < width);     // X坐标有效性检查
    COMPILE_TIME_ASSERT(y1 < height);   // Y起始坐标检查
    COMPILE_TIME_ASSERT(y2 < height);   // Y结束坐标检查
    COMPILE_TIME_ASSERT(y1 <= y2);      // 坐标顺序检查

    // 逐行设置位模式
    for(coord_t y = y1; y <= y2; ++y)
    {
        // 计算目标存储页（每8行为一页）
        const coord_t page = get_page(y);  // 等价于 y / 8

        // 生成位掩码（黑色对应位置1）
        const uint16_t bit_mask =get_mask(y);

        // 计算缓冲区索引
        const coord_t index = page * width + x;

        // 执行位操作（保证原子性）
        buf[index] |= bit_mask;
    }
}

#endif //ZQ_GUI_H
