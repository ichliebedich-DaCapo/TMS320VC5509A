//
// Created by fairy on 2025/3/14 23:57.
//
#ifndef GUI_WAVEFORM_HPP
#define GUI_WAVEFORM_HPP
#include <cmath>
#include <cstring>

#include"zq_gui.h"

/**
 * 示波器组件
 * @tparam MAX_POINT_NUM 波形的点数,默认与宽度相同,由于量化，宽度必须是最大点数的整数倍
 * @tparam x 起点x坐标
 * @tparam y 起点y坐标
 * @tparam width 波形区域的宽度，整个示波器组件实际宽度为width+2
 * @tparam height 波形区域的高度，整个示波器组件实际高度为height+2
 * @tparam H_GRIDS 水平网格数（垂直分割线数量）,由于量化原因，宽度必须是水平分割线的整数倍
 * @tparam V_GRIDS 纵向网格数（水平分割线数量），同上
 * @note 默认是滚动更新，当然，其他模式其实根本没做
 */
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM = width,
    uint16_t MAX_AMPLITUDE = 255, // 输入数据的最大幅度值
    uint16_t H_GRIDS = 4, // 横向网格数（垂直分割线数量）
    uint16_t V_GRIDS = 4> // 纵向网格数（水平分割线数量）
class WaveformView : public GUI_Object
{
public:
    void create() const;

protected:

    // 绘制波形函数
    static void draw();

    // 计算Y坐标转换（屏幕坐标系适配）
    static inline coord_t scale_y(int data)
    {
        return height - 1 - (data * (height - 1) / MAX_AMPLITUDE);
    }

private:
    static uint16_t buffer[MAX_POINT_NUM]; // 存储波形数据
    static uint16_t static_buffer[(width + 2) * GUI_PAGE_HEIGHT(y,height+2)]; // 存储静态网格数据，由于一页8位，高度需要除以8,向上取整
    static uint16_t index; // 波形数据索引
    static uint16_t state; // 状态
};

// =======================变量====================
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::buffer[MAX_POINT_NUM] = {};
// 存储波形数据
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS> uint16_t
WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::static_buffer[
    (width + 2) * GUI_PAGE_HEIGHT(y,height+2)] = {};
// 存储静态网格数据
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::index; // 波形数据索引
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::state;

// ==================================模板类的实现==========================================
// 创建函数
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::create() const
{
    GUI_Object::create(draw); // 里面可以塞入一个指针，然后把当前的draw保存到对象数组里

    // 绘制静态网格到指定缓冲区
    /* 基础边框 */
    const uint16_t start_y= y&0x07;// 为了更加细粒化控制y坐标
    const uint16_t end_y =start_y+height+1;
    draw_hline< static_buffer, width + 2, height + 2>(0, width + 1,start_y); // 顶边
    draw_hline< static_buffer, width + 2, height + 2>(0, width + 1, end_y); // 底边
    draw_vline<static_buffer, width + 2, height + 2>(start_y, end_y, 0); // 左边
    draw_vline< static_buffer, width + 2, height + 2>(start_y, end_y, width + 1); // 右边

    // /* 生成横向网格（垂直分割线） */
    uint16_t grid = height / (H_GRIDS + 1); // 横向网格间距
    uint16_t temp = grid+start_y;
    for (uint16_t i = 0; i < H_GRIDS; ++i)
    {
        draw_hline<static_buffer, width + 2, height + 2>(1, width, temp);
        temp += grid;
    }

    // /* 生成纵向网格（水平分割线） */
    grid = width / (V_GRIDS + 1); // 横向网格间距
    temp = grid;
    for (uint16_t i = 0; i < V_GRIDS; ++i)
    {
        draw_vline<static_buffer, width + 2, height + 2>(start_y+1, end_y-1, temp);
        temp += grid;
    }

    // 再把指定缓冲区的数据复制到显示缓冲区里
    buffer_copy<x, y, static_buffer, width+2, height+2>();
    printf("%d \n",GUI_PAGE_HEIGHT(y,height+2));
    // state = 1;
}

// 绘制波形函数
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::draw()
{
    if (state)
    {
        state = 0; // 状态复位

        // 绘制静态网格
        buffer_copy<x, y, static_buffer, width+2, height+2>(); // 使用复制缓冲区的办法来绘制静态网格

        // 绘制波形
        for (int j = 0; j < MAX_POINT_NUM; j++)
        {
            buffer[j] = static_cast<uint16_t>(32 * (sin(j * 0.2) + 1));
        }

        // 生成X坐标序列（均匀分布在水平方向）
        for (int i = 0; i < MAX_POINT_NUM - 1; ++i)
        {
            // 计算相邻点坐标
            const coord_t x0 = i;
            const coord_t y0 = scale_y(buffer[i]);
            const coord_t x1 = i + 1;
            const coord_t y1 = scale_y(buffer[i + 1]);

            // 调用模板化线段绘制
            draw_line(x0, y0, x1, y1, 1);
        }
    }
}


#endif //GUI_WAVEFORM_HPP
