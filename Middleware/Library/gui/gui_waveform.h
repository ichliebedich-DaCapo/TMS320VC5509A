//
// Created by fairy on 2025/3/14 23:57.
//
#ifndef GUI_WAVEFORM_HPP
#define GUI_WAVEFORM_HPP
#include <cstring>

#include"zq_gui.h"

/**
 * 示波器组件
 * @tparam MAX_POINT_NUM 波形的点数
 * @note 默认是滚动更新，当然，其他模式其实根本没做
 */
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS = 4, // 横向网格数（垂直分割线数量）
    uint16_t V_GRIDS = 4> // 纵向网格数（水平分割线数量）
class WaveformView : public GUI_Object
{
public:
    void create() const;

protected:
    // 绘制静态网格函数

    // 横向网格生成模板（带终止条件）
    template<uint16_t CURRENT, bool ENABLE = (CURRENT <= H_GRIDS)>
    struct HorizontalGridGenerator
    {
        static void draw()
        {
            const uint16_t x_pos = CURRENT * width / (H_GRIDS + 1);
            draw_vline<0, height - 1, x_pos, static_buffer, width, height>();
            HorizontalGridGenerator<CURRENT + 1>::draw();
        }
    };

    // 终止条件特化
    template<uint16_t CURRENT>
    struct HorizontalGridGenerator<CURRENT, false>
    {
        static void draw()
        {
        }
    };

    // 纵向网格生成模板（带终止条件）
    template<uint16_t CURRENT, bool ENABLE = (CURRENT <= V_GRIDS)>
    struct VerticalGridGenerator
    {
        static void draw()
        {
            const uint16_t y_pos = CURRENT * height / (V_GRIDS + 1);
            draw_hline<0, width - 1, y_pos, static_buffer, width, height>();
            VerticalGridGenerator<CURRENT + 1>::draw();
        }
    };

    // 终止条件特化
    template<uint16_t CURRENT>
    struct VerticalGridGenerator<CURRENT, false>
    {
        static void draw()
        {
        }
    };


    // 绘制波形函数
    static void draw();

private:
    static uint16_t buffer[MAX_POINT_NUM]; // 存储波形数据
    static uint16_t static_buffer[width * GUI_PAGE_HEIGHT(height)]; // 存储静态网格数据，由于一页8位，高度需要除以8,向上取整
    static uint16_t index; // 波形数据索引
    static uint16_t state; // 状态
};

// =======================变量====================
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::buffer[MAX_POINT_NUM] = {}; // 存储波形数据
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t
WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::static_buffer[width * GUI_PAGE_HEIGHT(height)] = {};
// 存储静态网格数据
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::index; // 波形数据索引
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::state;

// ==================================模板类的实现==========================================
// 创建函数
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::create() const
{
    GUI_Object::create(draw); // 里面可以塞入一个指针，然后把当前的draw保存到对象数组里

    // 绘制静态网格到指定缓冲区
    /* 基础边框 */
    draw_hline<0, width - 1, 0, static_buffer, width, height>(); // 顶边
    draw_hline<0, width - 1, height - 1, static_buffer, width, height>(); // 底边
    draw_vline<0, height - 1, 0, static_buffer, width, height>(); // 左边
    draw_vline<0, height - 1, width - 1, static_buffer, width, height>(); // 右边

    /* 生成横向网格（垂直分割线） */
    HorizontalGridGenerator<1>::draw();

    /* 生成纵向网格（水平分割线） */
    VerticalGridGenerator<1>::draw();

    // 再把指定缓冲区的数据复制到显示缓冲区里
    buffer_copy<x, y, static_buffer, width, height>();
}

// 绘制波形函数
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<MAX_POINT_NUM, x, y, width, height, H_GRIDS, V_GRIDS>::draw()
{
    if (state)
    {
        // 绘制静态网格
        buffer_copy<x, y, static_buffer, width, height>(); // 使用复制缓冲区的办法来绘制静态网格

        // 绘制波形

    }
}


#endif //GUI_WAVEFORM_HPP
