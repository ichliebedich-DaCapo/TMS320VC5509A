//
// Created by fairy on 2025/3/13 10:55.
//
#ifndef ZQ_GUI_H
#define ZQ_GUI_H

#ifdef SIMULATOR
#include<stdint.h>
#else
#include <oled.h>
#endif
#include<zq_font.h>
#include<zq_image.h>
#include <cstdio>
#include <cstring>


// 属性声明
#define GUI_HOR 128
#define GUI_VOR 64
#define GUI_PAGE 8
#define color_white 0
#define color_black 1


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
            const uint16_t mask = (1) << shift; // 确保类型匹配
            const bool ret = flag & mask; // 直接转换为bool
            flag &= ~mask; // 清除标记位
            return ret;
        }

        INLINE bool read() { return flag & (1 << shift); } // 仅读取标志
    };

    namespace Flag
    {
        // 渲染标志
        class render : public FlagType<0> {};

        // 更新模式标志，0为默认模式，全刷新  1为分页模式，分页刷新
        class partialUpdate : public FlagType<1> {};

        // 用户绘制界面标志
        class draw : public FlagType<2> {};
    }
}


namespace GUI
{
    // 数据基类
    class Base
    {
    protected:
        static unsigned char buffer[GUI_PAGE][GUI_HOR]; // 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
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

        INLINE void clear(uint16_t start_page, uint16_t end_page, uint16_t start_col, uint16_t end_col); // 区域清除

        INLINE void clear(); // 清除整个屏幕的显存

        static void draw_hline(uint16_t x_start, uint16_t x_end, uint16_t y); // 绘制水平线

        static void draw_vline(uint16_t x, uint16_t y_start, uint16_t y_end); // 绘制垂直线

        static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // Bresenham直线算法

        template<uint16_t steps>
        static void draw_catmull_rom(Point p0, Point p1, Point p2, Point p3); // 绘制Catmull-Rom样条曲线（需要4个连续控制点）


        template<uint16_t steps>
        static void draw_bezier3(Point p0, Point p1, Point p2, Point p3); // 绘制三阶贝塞尔曲线（需要4个连续控制点）


        static void draw_char(const char *name, uint16_t x, uint16_t y, const Font::FontChar fonts[]); // 自动查找字符元数据

        static void draw_string(const char *str, uint16_t x, uint16_t y, const Font::FontChar fonts[]);
    };


    class Render : Base
    {
        static uint16_t refresh_page; // 类级别静态变量
    protected:
        static void screen(); // 界面函数，由用户自己实现
        static void draw(); // 绘制函数，由用户自己实现
    public:
        template<void(*oled_init)()>
        static void init()
        {
            oled_init(); // 初始化OLED驱动
            Tools::clear(); // 清屏
            screen(); // 初始化界面
            Flag::render::set(); // 设置渲染标志位
        }


        template<void(*oled_write_data)(unsigned char page, const unsigned char *buf)>
        static void handler()
        {
            // 绘制处理
            draw();

            // ==========刷新处理==========
            if (Flag::render::read())
            {
                if (Flag::partialUpdate::read())
                {
                    // 分页模式，分页刷新
                    oled_write_data(refresh_page, buffer[refresh_page]);
                    refresh_page = (++refresh_page) & 0x07;

                    if (refresh_page == 0)
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
    void Tools::write_pixel(const uint16_t x, const uint16_t y, const bool color)
    {
        if (x >= GUI_HOR || y >= GUI_VOR) return;

        const uint16_t page = y >> 3;
        const unsigned char bit = y & 0x07;
        const unsigned char mask = 1 << bit;

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
        const unsigned char bit = y & 0x07;
        buffer[page][x] |= (1 << bit);
    }

    /**
     * @brief 清除指定区域的显存
     * @param start_page 起始页（基于纵向分页）
     * @param end_page 结束页
     * @param start_col 起始列
     * @param end_col 结束列
     */
    void Tools::clear(const uint16_t start_page, const uint16_t end_page,
                      const uint16_t start_col, const uint16_t end_col)
    {
        for (uint16_t page = start_page; page <= end_page; ++page)
        {
            const uint16_t length = end_col - start_col + 1;
            std::memset(buffer[page] + start_col, 0, length * sizeof(unsigned char));
        }
    }

    // 全屏清除
    void Tools::clear()
    {
        std::memset(&buffer[0][0], 0, GUI_HOR * GUI_PAGE * sizeof(unsigned char));
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
