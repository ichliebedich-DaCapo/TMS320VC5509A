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

// 属性声明
#define GUI_HOR 128 // 2^7，考虑到一行正好是2^7，那么就可以通过移位代替乘法，提高效率
#define GUI_VOR 64
#define GUI_PAGE 8
#define color_white 0
#define color_black 1
#define DIRTY_DUMMY_COL 0x00FF    // 无效脏列，用于重置脏标记

namespace GUI
{
    // ====================数据类型声明=====================
    // 类型别名
    typedef uint16_t coord_t; // 坐标类型

    // 点结构体
    typedef struct Point
    {
        coord_t x;
        coord_t y;
    } Point;
}


namespace GUI
{
    // 适配C55x系列的标志类
    class FlagBase
    {
    protected:
        static uint16_t flag;
    };

    // 标志类
    template<uint16_t shift>
    class FlagType:public FlagBase
    {
    public:
        INLINE void set(){flag|=(1<<shift); }// 设置标志
        INLINE void reset(){flag&=~(1<<shift);}// 重置标志
        INLINE bool get() {return flag&(1<<shift);}// 获取标志
    };

    namespace Flag
    {
        class render:public FlagType<0>{};// 渲染标志
        class updateMode:public FlagType<1>{};// 更新模式标志，0为默认模式，全刷新  1为分页模式，分页刷新
        class draw:public FlagType<2>{};// 用户绘制界面标志

    }
}


// 初始化函数
namespace GUI
{
    void init();
}




namespace GUI
{
    // 基类
    class Base
    {
    protected:
        static uint16_t buffer[GUI_PAGE][GUI_HOR]; // 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
    };

    class Tools:Base
    {
    public:
        // 绘制像素点（黑白都有）
        INLINE void write_pixel(const uint16_t x, const uint16_t y, const bool color)
        {
            if (x >= GUI_HOR || y >= GUI_VOR) return;

            const uint16_t page = y >> 3;
            const uint16_t bit = y & 0x07;
            const uint16_t mask = 1 << bit;
            if (color) {
                buffer[page][x] |= mask;
            } else {
                buffer[page][x] &= ~mask;
            }
        }

        // 绘制像素点(默认黑色）
        INLINE void write_pixel(const uint16_t x, const uint16_t y)
        {
            if (x >= GUI_HOR || y >= GUI_VOR) return;

            const uint16_t page = y >> 3;
            const uint16_t bit = y & 0x07;
            const uint16_t mask = 1 << bit;
            buffer[page][x] |= mask;
        }

        // 绘制横线(默认黑色)
        INLINE void draw_hline(uint16_t x_start, uint16_t x_end, const uint16_t y)
        {
            if (y >= GUI_VOR) return;  // 越界检查
            if (x_start > x_end) swap(x_start, x_end);  // 确保顺序

            // 限制坐标在屏幕范围内
            x_start = (x_start >= GUI_HOR) ? GUI_HOR-1 : x_start;
            x_end = (x_end >= GUI_HOR) ? GUI_HOR-1 : x_end;

            const uint16_t page = y >> 3;         // 计算所在页
            const uint16_t mask = 1 << (y & 0x07); // 生成位掩码

            // 批量设置该行所有列的对应位
            for (uint16_t x = x_start; x <= x_end; ++x) {
                buffer[page][x] |= mask;
            }
        }

        // 绘制竖线(默认黑色)
        INLINE void draw_vline(const uint16_t x, uint16_t y_start, uint16_t y_end)
        {
            if (x >= GUI_HOR) return;  // 越界检查
            if (y_start > y_end) swap(y_start, y_end);  // 确保顺序

            // 限制坐标在屏幕范围内
            y_start = (y_start >= GUI_VOR) ? GUI_VOR-1 : y_start;
            y_end = (y_end >= GUI_VOR) ? GUI_VOR-1 : y_end;

            const uint16_t start_page = y_start >> 3;  // 起始页
            const uint16_t end_page = y_end >> 3;      // 结束页

            // 逐页设置位掩码
            for (uint16_t page = start_page; page <= end_page; ++page)
            {
                uint16_t bit_mask = 0xFF;  // 默认全页覆盖
                if (page == start_page) {  // 起始页的特殊处理
                    bit_mask &= (0xFF << (y_start & 0x07));
                }
                if (page == end_page) {    // 结束页的特殊处理
                    bit_mask &= (0xFF >> (7 - (y_end & 0x07)));
                }
                buffer[page][x] |= bit_mask;  // 应用位掩码
            }
        }

        // 绘制直线(默认黑色，Bresenham算法优化)
        INLINE void draw_line(uint16_t x0, uint16_t y0, const uint16_t x1, const uint16_t y1)
        {
            // 坐标越界直接返回
            if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

            const int16_t dx = abs_diff(x0,x1);
            const int16_t dy = -abs_diff(y0,y1);
            const int16_t sx = (x0 < x1) ? 1 : -1;
            const int16_t sy = (y0 < y1) ? 1 : -1;
            int16_t err = dx + dy;

            while (true)
            {
                write_pixel(x0, y0);  // 调用默认黑色像素绘制
                if (x0 == x1 && y0 == y1) break;
                const int16_t e2 =err<<1;
                if (e2 >= dy) {  // 水平步进
                    err += dy;
                    x0 += sx;
                }
                if (e2 <= dx) {  // 垂直步进
                    err += dx;
                    y0 += sy;
                }
            }
        }

        INLINE void clear(const uint16_t start_page, const uint16_t end_page, const uint16_t start_col,uint16_t end_col)
        {
            for (uint16_t page = start_page; page <= end_page; ++page)
            {
                memset(buffer[page]+start_col, 0, sizeof((end_col - start_col+1)*sizeof(uint16_t)));
            }
        }

        INLINE void clear()
        {
                memset(&buffer[0][0], 0, sizeof(GUI_HOR*GUI_PAGE*sizeof(uint16_t)));
        }

        // Catmull-Rom样条（需要4个连续点）
    template<uint16_t steps>
    static void draw_catmull_rom(Point p0, Point p1, Point p2, Point p3);

        // 三阶贝塞尔曲线（需要4个连续点）
       template<uint16_t steps>
static void draw_bezier3(Point p0, Point p1, Point p2, Point p3);

    };

    class Render:Base
    {
    public:
        static void init();// 初始化函数，由用户自己实现
        static void draw();// 绘制函数，由用户自己实现

        template<void(*oled_write_data)(uint16_t page, const uint16_t *buf)>
        static void handler()
        {
            // 绘制处理
            draw();

            // ==========刷新处理==========
            if (Flag::render::get())
            {
                if (Flag::updateMode::get())
                {
                    // 分页模式，分页刷新
                    static uint16_t page = 0;
                    oled_write_data(page, buffer[page]);
                    page = (++page)&0x07;

                    if (page == 0)
                        Flag::render::reset();
                }
                else
                {
                    Flag::render::reset();
                    // 全部刷新
                    for (uint16_t page = 0; page < GUI_PAGE; ++page)
                    {
                        oled_write_data(page, buffer[page]);
                    }
                }

            }

        }
    };

}





namespace GUI
{
template<uint16_t steps>
// Catmull-Rom样条（需要4个连续点）
void Tools::draw_catmull_rom(const Point p0, const Point p1, const Point p2, const Point p3)
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
void Tools::draw_bezier3(const Point p0, const Point p1, const Point p2, const Point p3)
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
}
#endif //ZQ_GUI_H
