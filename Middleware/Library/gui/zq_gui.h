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


// 根据高度，获得所需的页数，用于定义数组
#define GUI_PAGE_HEIGHT(y0,h) ((((y0) + (h) + 7) >> 3) - ((y0) >>3))

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

// 点结构体
typedef struct Point
{
    coord_t x;
    coord_t y;
} Point;


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
        return (page << 7) + x; // 8页的情况下
#else
        return (page <<6) + x;// 16页的情况下
#endif
    }

    INLINE coord_t Index_xy(const uint16_t &x, uint16_t y)
    {
#if GUI_PAGE_MODE == 8
        y = get_page(y); // 获取页数
        return (y << 7) + x; // 8页的情况下
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
    INLINE void update_dirty_col(const uint16_t page, const uint16_t min_col, const uint16_t max_col)
    {
        dirty_info.is_dirty |= 1 << page;
        if (min_col < get_min_col(page)) set_min_col(page, min_col);
        if (max_col > get_max_col(page)) set_max_col(page, max_col);
    }

    // 根据y坐标获取page
    // 128*64分成左右两个页区，左页区有0、2、4等页，由页区有1、3、5等页。由x坐标来区分左右页
#if GUI_PAGE_MODE == 8
    INLINE uint16_t get_page(const uint16_t &y)
    {
        return (y >> 3);
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
    template<uint16_t x, uint16_t y, uint16_t *buf, uint16_t width, uint16_t height>
    static void buffer_copy()
    {
        for (uint16_t i = 0; i < GUI_PAGE_HEIGHT(y, height); ++i)
        {
            // 一行一行地传输
            memcpy(buffer + (((y >> 3) + i) << 7) + x, buf + i * width, width * sizeof(uint16_t));
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
    static void create(const DrawFunc &draw)
    {
        obj_list[count] = draw;
        ++count;
    } // 没有进行判断是否越界，因为目前的界面非常简单，暂时没必要
    static void destroy() { --count; }


    // =======基本绘制函数======
    INLINE void write_pixel(uint16_t x, uint16_t y, uint16_t data);

    INLINE void write_pixel(uint16_t x, uint16_t y); // 绘制黑色

    static void draw_hline(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);

    static void draw_vline(uint16_t y1, uint16_t y2, uint16_t x, uint16_t color);

    static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // 默认绘制黑色

    static void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    static void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

    // ============绘制曲线算法===========
    // 样条算法
    template<uint16_t steps >
    static void draw_catmull_rom(Point p0, Point p1, Point p2, Point p3);

    // 3次贝塞尔曲线算法
    template<uint16_t steps >
    static void draw_bezier3(Point p0, Point p1, Point p2, Point p3);


    // 指定缓冲区 模板化 默认黑色
    template<uint16_t * buf, uint16_t width, uint16_t height>
    static void draw_hline(coord_t x1, coord_t x2, coord_t y);

    template<uint16_t * buf, coord_t width, coord_t height>
    static void draw_vline(coord_t y1, coord_t y2, coord_t x);

    template<uint16_t * buf, coord_t width, coord_t height>
    static void draw_line(coord_t x0, coord_t y0, coord_t x1, coord_t y1);


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

void GUI_Object::write_pixel(const uint16_t x, const uint16_t y)
{
    if (x >= GUI_HOR || y >= GUI_VOR) return;

#if GUI_PAGE_MODE == 8
    const uint16_t page = get_page(y);
#else
    const uint16_t page = get_page(x, y);
#endif


    // 原子操作更新缓冲区
    buffer[Index(page, x)] |= get_mask(y);
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
        oled_write_data(page, get_min_col(page), get_max_col(page), buffer + Index(page, get_min_col(page)));
        reset_dirty(page); // 重置脏标记
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

template<uint16_t * buf, coord_t width, coord_t height>
void GUI_Object::draw_hline(coord_t x1, coord_t x2, coord_t y)
{
    // 编译期坐标安全检查（C++98兼容方案）
    COMPILE_TIME_ASSERT(y < height, "y坐标超出范围");
    COMPILE_TIME_ASSERT(x1 < width,"x1坐标超出范围");
    COMPILE_TIME_ASSERT(x2 < width, "x2坐标超出范围");
    COMPILE_TIME_ASSERT(x1 <= x2,"x1坐标必须小于等于x2");

    // 计算目标存储页（每页8行）
    const coord_t page = get_page(y); // 等价于 y / 8

    // 生成目标位的掩码（黑色对应置1）
    const uint16_t bit_mask = get_mask(y);

    // 线性缓冲区访问模式
    const coord_t start_index = page * width;
    for (coord_t x = x1; x <= x2; ++x)
    {
        // 计算目标存储单元在缓冲区中的位置
        buf[start_index + x] |= bit_mask;
    }
}

template<uint16_t * buf, coord_t width, coord_t height>
void GUI_Object::draw_vline(coord_t y1, coord_t y2, coord_t x)
{
    COMPILE_TIME_ASSERT(x < width, "x坐标超出范围"); // X坐标有效性检查
    COMPILE_TIME_ASSERT(y1 < height,"y1坐标超出范围"); // Y起始坐标检查
    COMPILE_TIME_ASSERT(y2 < height,"y2坐标超出范围"); // Y结束坐标检查
    COMPILE_TIME_ASSERT(y1 <= y2,"y1坐标必须小于等于y2"); // 坐标顺序检查

    // 逐行设置位模式
    for (coord_t y = y1; y <= y2; ++y)
    {
        // 计算目标存储页（每8行为一页）
        const coord_t page = get_page(y); // 等价于 y / 8

        // 生成位掩码（黑色对应位置1）
        const uint16_t bit_mask = get_mask(y);

        // 计算缓冲区索引
        const coord_t index = page * width + x;

        // 执行位操作（保证原子性）
        buf[index] |= bit_mask;
    }
}


template<
    uint16_t * buf, // 显示缓冲区
    coord_t width, // 屏幕宽度
    coord_t height // 屏幕高度
>
void GUI_Object::draw_line(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
{
    // 编译期坐标安全检查
    COMPILE_TIME_ASSERT(x0 < width && x1 < width, "x坐标超出范围");
    COMPILE_TIME_ASSERT(y0 < height && y1 < height,"y坐标超出范围");



    // Bresenham 算法参数计算（编译期常量）
    const coord_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    const coord_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    const coord_t sx = (x0 < x1) ? 1 : -1;
    const coord_t sy = (y0 < y1) ? 1 : -1;

    // 运行时变量（需要保留算法逻辑）
    coord_t x = x0;
    coord_t y = y0;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true)
    {
        // 处理当前像素点
        const coord_t page = y >> 3;
        const uint16_t mask = 1 << (y & 0x7);
        const coord_t index = page * width + x;

        // 根据颜色模板参数选择操作
        buf[index] |= mask;


        // 终止条件
        if (x == x1 && y == y1) break;

        // 更新步进
        const int e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y += sy;
        }
    }
}

template<uint16_t steps>
// Catmull-Rom样条（需要4个连续点）
void GUI_Object::draw_catmull_rom(const Point p0, const Point p1, const Point p2, const Point p3)
{
    for (uint16_t i = 0; i <= steps; ++i)
    {
        const float t = static_cast<float>(i) / steps;
        const float t2 = t * t;
        const float t3 = t2 * t;

        const float x = 0.5f * ((-t3 + 2 * t2 - t) * p0.x +
                                (3 * t3 - 5 * t2 + 2) * p1.x +
                                (-3 * t3 + 4 * t2 + t) * p2.x +
                                (t3 - t2) * p3.x);
        const float y = 0.5f * ((-t3 + 2 * t2 - t) * p0.y +
                                (3 * t3 - 5 * t2 + 2) * p1.y +
                                (-3 * t3 + 4 * t2 + t) * p2.y +
                                (t3 - t2) * p3.y);
        write_pixel(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
    }
}

// B-Spline样条（需要4个连续点）
/**
 * 三次贝塞尔曲线绘制（整数运算优化版）
 * @param p0 控制点
 * @param p1 控制点
 * @param p2 控制点
 * @param p3 控制点
 * @tparam steps 采样点数（越多越平滑，但性能越低）
 */
template<uint16_t steps>
void GUI_Object::draw_bezier3(const Point p0, const Point p1, const Point p2, const Point p3)
{
    for (uint16_t i = 0; i <= steps; ++i) {
        // 计算t的范围[0.0, 1.0]
        const float t = static_cast<float>(i) / static_cast<float>(steps);
        const float u = 1.0f - t;

        // 预计算t的幂次减少乘法次数
        const float t2 = t * t;
        const float t3 = t2 * t;
        const float u2 = u * u;
        const float u3 = u2 * u;

        // 贝塞尔公式展开
        const float B0 = u3;
        const float B1 = 3 * u2 * t;
        const float B2 = 3 * u * t2;
        const float B3 = t3;

        // 计算实际坐标（四舍五入取整）
        const uint16_t x = static_cast<uint16_t>(B0 * p0.x + B1 * p1.x + B2 * p2.x + B3 * p3.x + 0.5f);
        const uint16_t y = static_cast<uint16_t>(B0 * p0.y + B1 * p1.y + B2 * p2.y + B3 * p3.y + 0.5f);

        // 安全绘制（避免坐标越界）
            write_pixel(x, y, 1); // 1表示黑色

    }
}

#endif //ZQ_GUI_H
