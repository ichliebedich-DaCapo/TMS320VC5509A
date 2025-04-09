//
// Created by fairy on 2025/3/13 10:55.
//
#ifndef ZQ_GUI_H
#define ZQ_GUI_H

#ifdef SIMULATOR
#include<stdint.h>
#include <zq_conf.h>
#include<zq_font.h>
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
    class FlagType : public FlagBase
    {
    public:
        INLINE void set() { flag |= (1 << shift); } // 设置标志
        INLINE void reset() { flag &= ~(1 << shift); } // 重置标志
        // 获取标志，并清除
        INLINE bool get()
        {
            const uint16_t mask = (1) << shift;  // 确保类型匹配
            const bool ret = flag & mask;       // 直接转换为bool
            flag &= ~mask;                      // 清除标记位
            return ret;
        }
        INLINE bool read(){ return flag & (1 << shift);}// 仅读取标志
    };

    namespace Flag
    {
        class render : public FlagType<0>
        {
        }; // 渲染标志
        class updateMode : public FlagType<1>
        {
        }; // 更新模式标志，0为默认模式，全刷新  1为分页模式，分页刷新
        class draw : public FlagType<2>
        {
        }; // 用户绘制界面标志
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






    /**
 * @brief 图形绘制工具类，提供基础绘图功能
 * @note 继承自Base类，使用显存缓冲进行绘制操作
 */
    class Tools : public Base
    {
    public:
        INLINE void write_pixel(uint16_t x, uint16_t y, bool color); // 绘制像素点（支持黑白）

        INLINE void write_pixel(uint16_t x, uint16_t y); // 绘制黑色像素点（默认）

        INLINE void draw_hline(uint16_t x_start, uint16_t x_end, uint16_t y); // 绘制水平线

        INLINE void draw_vline(uint16_t x, uint16_t y_start, uint16_t y_end); // 绘制垂直线

        INLINE void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // Bresenham直线算法

        INLINE void clear(uint16_t start_page, uint16_t end_page, uint16_t start_col, uint16_t end_col); // 区域清除

        INLINE void clear(); // 清除整个屏幕的显存


        template<uint16_t steps>
        static void draw_catmull_rom(Point p0, Point p1, Point p2, Point p3); // 绘制Catmull-Rom样条曲线（需要4个连续控制点）


        template<uint16_t steps>
        static void draw_bezier3(Point p0, Point p1, Point p2, Point p3); // 绘制三阶贝塞尔曲线（需要4个连续控制点）


        // 新函数：自动查找字符元数据
        static void draw_char(const char* name, const uint16_t x, const uint16_t y) {
            const Font::FontChar* fc = Font::find_char_by_name(name);
            if (!fc) return;

            const uint16_t* font_data = fc->data;
            const uint16_t char_width = fc->width;
            const uint16_t char_height = fc->height;

            for (uint16_t row_char = 0; row_char < char_height; row_char++) {
                const uint16_t byte = font_data[row_char]; // 强制转换为uint8_t

                for (uint16_t bit = 0; bit < char_width; bit++) {
                    if (byte & (1 << bit)) {
                        const uint16_t y_total = y + row_char;
                        const uint16_t x_total = x + bit;

                        if (y_total < 0 || y_total >= 64) continue;
                        if (x_total < 0 || x_total >= 128) continue;

                        const uint16_t page = y_total >>3;
                        const uint16_t row_in_page = y_total & 0x07;
                        buffer[page][x_total] |= (1 << row_in_page);
                    }
                }
            }
        }
    };


    class Render : Base
    {
    public:
        static void init(); // 初始化函数，由用户自己实现
        static void draw(); // 绘制函数，由用户自己实现

        template<void(*oled_write_data)(uint16_t page, const uint16_t *buf)>
        static void handler()
        {
            // 绘制处理
            draw();

            // ==========刷新处理==========
            if (Flag::render::read())
            {
                if (Flag::updateMode::read())
                {
                    // 分页模式，分页刷新
                    static uint16_t page = 0;
                    oled_write_data(page, buffer[page]);
                    page = (++page) & 0x07;

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
    /**
     * @brief 绘制像素点（支持黑白两色）
     * @param x 横坐标（0起始）
     * @param y 纵坐标（0起始）
     * @param color 颜色标识，true为黑色，false为白色
     */
    void Tools::write_pixel(uint16_t x, uint16_t y, bool color)
    {
        if (x >= GUI_HOR || y >= GUI_VOR) return;

        const uint16_t page = y >> 3;
        const uint8_t bit = y & 0x07;
        const uint8_t mask = 1 << bit;

        color ? (buffer[page][x] |= mask) : (buffer[page][x] &= ~mask);
    }


    /**
     * @brief 绘制黑色像素点（默认颜色重载）
     * @param x 横坐标（0起始）
     * @param y 纵坐标（0起始）
     */
    void Tools::write_pixel(const uint16_t x, const uint16_t y)
    {
        if (x >= GUI_HOR || y >= GUI_VOR) return;

        const uint16_t page = y >> 3;
        const uint8_t bit = y & 0x07;
        buffer[page][x] |= (1 << bit);
    }


    /**
    * @brief 绘制水平线段（默认黑色）
    * @param x_start 起始点横坐标
    * @param x_end 结束点横坐标
    * @param y 固定纵坐标
    */
    void Tools::draw_hline(uint16_t x_start, uint16_t x_end, const uint16_t y)
    {
        if (y >= GUI_VOR) return; // 越界检查
        if (x_start > x_end) swap(x_start, x_end); // 确保顺序

        // 限制坐标在屏幕范围内
        x_start = (x_start >= GUI_HOR) ? GUI_HOR - 1 : x_start;
        x_end = (x_end >= GUI_HOR) ? GUI_HOR - 1 : x_end;

        const uint16_t page = y >> 3; // 计算所在页
        const uint16_t mask = 1 << (y & 0x07); // 生成位掩码

        // 批量设置该行所有列的对应位
        for (uint16_t x = x_start; x <= x_end; ++x)
        {
            buffer[page][x] |= mask;
        }
    }


    /**
    * @brief 绘制垂直线段（默认黑色）
    * @param x 固定横坐标
    * @param y_start 起始点纵坐标
    * @param y_end 结束点纵坐标
    */
    void Tools::draw_vline(const uint16_t x, uint16_t y_start, uint16_t y_end)
    {
        if (x >= GUI_HOR) return; // 越界检查
        if (y_start > y_end) swap(y_start, y_end); // 确保顺序

        // 限制坐标在屏幕范围内
        y_start = (y_start >= GUI_VOR) ? GUI_VOR - 1 : y_start;
        y_end = (y_end >= GUI_VOR) ? GUI_VOR - 1 : y_end;

        const uint16_t start_page = y_start >> 3; // 起始页
        const uint16_t end_page = y_end >> 3; // 结束页

        // 逐页设置位掩码
        for (uint16_t page = start_page; page <= end_page; ++page)
        {
            uint16_t bit_mask = 0xFF; // 默认全页覆盖
            if (page == start_page)
            {
                // 起始页的特殊处理
                bit_mask &= (0xFF << (y_start & 0x07));
            }
            if (page == end_page)
            {
                // 结束页的特殊处理
                bit_mask &= (0xFF >> (7 - (y_end & 0x07)));
            }
            buffer[page][x] |= bit_mask; // 应用位掩码
        }
    }


    /**
       * @brief 使用Bresenham算法绘制直线（默认黑色）
       * @param x0 起点横坐标
       * @param y0 起点纵坐标
       * @param x1 终点横坐标
       * @param y1 终点纵坐标
       */
    void Tools::draw_line(uint16_t x0, uint16_t y0, const uint16_t x1, const uint16_t y1)
    {
        // 坐标越界直接返回
        if (x0 >= GUI_HOR || y0 >= GUI_VOR || x1 >= GUI_HOR || y1 >= GUI_VOR) return;

        const int16_t dx = abs_diff(x0, x1);
        const int16_t dy = -abs_diff(y0, y1);
        const int16_t sx = (x0 < x1) ? 1 : -1;
        const int16_t sy = (y0 < y1) ? 1 : -1;
        int16_t err = dx + dy;

        while (true)
        {
            write_pixel(x0, y0); // 调用默认黑色像素绘制
            if (x0 == x1 && y0 == y1) break;
            const int16_t e2 = err << 1;
            if (e2 >= dy)
            {
                // 水平步进
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                // 垂直步进
                err += dx;
                y0 += sy;
            }
        }
    }


    /**
     * @brief 清除指定区域的显存
     * @param start_page 起始页（基于纵向分页）
     * @param end_page 结束页
     * @param start_col 起始列
     * @param end_col 结束列
     */
    void Tools::clear(uint16_t start_page, uint16_t end_page,
                      uint16_t start_col, uint16_t end_col)
    {
        for (uint16_t page = start_page; page <= end_page; ++page)
        {
            const uint16_t length = end_col - start_col + 1;
            memset(buffer[page] + start_col, 0, length * sizeof(uint16_t));
        }
    }

    // 全屏清除
    void Tools::clear()
    {
        memset(&buffer[0][0], 0, GUI_HOR * GUI_PAGE * sizeof(uint16_t));
    }


    /**
     * @brief 绘制Catmull-Rom样条曲线（需要4个连续控制点）
     * @tparam steps 曲线分段数
     */
    template<uint16_t steps>
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
        for (uint16_t i = 0; i <= steps; ++i)
        {
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
