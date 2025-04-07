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
 * @tparam MAX_POINT_NUM 波形的点数,默认与宽度相同,满足 (N-1)*grid=P-1，P是总点数，grid是两波形数据点中间的点数+1，N是波形点数
 * @tparam x 起点x坐标
 * @tparam y 起点y坐标
 * @tparam width 波形区域的宽度，整个示波器组件实际宽度为width+2
 * @tparam height 波形区域的高度，整个示波器组件实际高度为height+2
 * @tparam H_GRIDS 水平网格数（垂直分割线数量）,由于量化原因，必须让公式 间距 = (W-1)/(N+1)，其中W为宽度
 * @tparam V_GRIDS 纵向网格数（水平分割线数量），同上
 * @note 默认是滚动更新，当然，其他模式其实根本没做
 */
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM = width,
    uint16_t MAX_AMPLITUDE = 255, // 输入数据的最大幅度值
    uint16_t H_GRIDS = 3, // 横向网格数（垂直分割线数量）
    uint16_t V_GRIDS = 3> // 纵向网格数（水平分割线数量）
class WaveformView : public GUI_Object
{
public:
    void create() const;

    // 添加下一点数据
    void set_next_value(uint16_t value) const
    {
        set_state();
        data_buffer[index] = value;
        index = (index + 1) % MAX_POINT_NUM;
        if (!is_buffer_full && index == 0)
            is_buffer_full = true;
    }

    // 绘制波形函数
    static void draw();
protected:
    // 计算Y坐标转换（屏幕坐标系适配）
    INLINE coord_t scale_y(const int data)
    {
        return y + height - (data * height / MAX_AMPLITUDE);
    }

    // 获取处理之后的y坐标
    INLINE coord_t get_y(const uint16_t i)
    {
        return scale_y(data_buffer[(i+index)%MAX_POINT_NUM]);
    }

    // 设置标志
    INLINE void set_state() { state = 1; }
    INLINE void clear_state() { state = 0; }
    INLINE bool get_state() { return state; }

private:
    static uint16_t data_buffer[MAX_POINT_NUM]; // 存储波形数据
    static uint16_t static_buffer[(width + 2) * GUI_PAGE_HEIGHT(y, height+2)]; // 存储静态网格数据，由于一页8位，高度需要除以8,向上取整
    static uint16_t index; // 波形数据索引
    static uint16_t is_buffer_full;
    static uint16_t state; // 状态
};

// =======================变量====================
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::data_buffer[MAX_POINT_NUM] =
        {};
// 存储波形数据
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t
WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::static_buffer[
    (width + 2) * GUI_PAGE_HEIGHT(y, height+2)] = {};
// 存储静态网格数据
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::index; // 波形数据索引
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::is_buffer_full = false;
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
uint16_t WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::state;

// ==================================模板类的实现==========================================
/**
 * 创建波形组件
 * @note 主要作用是创建静态网格。他宝贝的，为了更加细粒化控制y坐标耗费了不少脑细胞。
 */
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::create() const
{
    GUI_Object::create(draw); // 里面可以塞入一个指针，然后把当前的draw保存到对象数组里

    // 绘制静态网格到指定缓冲区
    /* 基础边框 */
    const uint16_t start_y = y & 0x07; // 为了更加细粒化控制y坐标
    const uint16_t end_y = start_y + height + 1;
    draw_hline<static_buffer, width + 2, height + 2>(0, width + 1, start_y); // 顶边
    draw_hline<static_buffer, width + 2, height + 2>(0, width + 1, end_y); // 底边
    draw_vline<static_buffer, width + 2, height + 2>(start_y, end_y, 0); // 左边
    draw_vline<static_buffer, width + 2, height + 2>(start_y, end_y, width + 1); // 右边

    // /* 生成横向网格（垂直分割线） */
    uint16_t grid = height / (H_GRIDS + 1); // 横向网格间距
    uint16_t temp = grid + start_y;
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
        draw_vline<static_buffer, width + 2, height + 2>(start_y + 1, end_y - 1, temp);
        temp += grid;
    }

    // 再把指定缓冲区的数据复制到显示缓冲区里
    buffer_copy<x, y, static_buffer, width + 2, height + 2>();
}


/**
 * 绘制波形函数，默认滚动更新。
 * @note 主要思路是，把静态网格的缓冲区直接复制到显示缓冲区里，相当于擦除，然后在此基础上，调用绘线函数，在显示缓冲区里绘制。
 */
template<uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t MAX_POINT_NUM, uint16_t MAX_AMPLITUDE,
    uint16_t H_GRIDS, uint16_t V_GRIDS>
void WaveformView<x, y, width, height, MAX_POINT_NUM, MAX_AMPLITUDE, H_GRIDS, V_GRIDS>::draw()
{
    if (get_state())
    {
        clear_state(); // 状态复位

        // 绘制静态网格
        buffer_copy<x, y, static_buffer, width + 2, height + 2>(); // 使用复制缓冲区的办法来绘制静态网格

        // 生成X坐标序列（均匀分布在水平方向）
        const uint16_t valid_points = is_buffer_full ? MAX_POINT_NUM : index;// 防止曲线突然跌落
        const uint16_t grid = (width-1)/(MAX_POINT_NUM-1);// 从一个点到相邻的后一个点的间隔

        // ==================绘制算法===================
// #define Linear_Inline 1
//
// #if Linear_Inline
//         // 线性插值算法
//         // 峰值附近会有空缺，不知道为什么
//         coord_t x0 = x + 1;
//         coord_t y0 = get_y(0);
//         for (uint16_t i = 0; i < valid_points-1; ++i)
//         {
//             // 计算相邻点坐标
//             const coord_t x1 = x0 + grid;
//             const coord_t y1 =get_y(i+1);
//
//             // 线段绘制
//             draw_line(x0, y0, x1, y1);
//             x0 += grid;
//             y0 = y1;
//         }
// #else
//         // 样条算法
//         if (valid_points<4)return;
//         for (uint16_t i = 0; i < valid_points - 3; ++i)
//         {
//             const Point p0={
//                 static_cast<uint16_t>(x + 1 + grid * i),
//                 get_y(i)
//             };
//             const Point p1={
//                 static_cast<uint16_t>(x + 1 + grid * (i+1)),
//                 get_y(i+1)
//             };
//             const Point p2={
//                 static_cast<uint16_t>(x + 1 + grid * (i+2)),
//                 get_y(i+2)
//             };
//             const Point p3={
//                 static_cast<uint16_t>(x + 1 + grid * (i+3)),
//                 get_y(i+3)
//             };
//
//             draw_catmull_rom<8>(p0, p1, p2, p3);
//             // draw_bezier3<8>(p0, p1, p2, p3);
//         }
// #endif
// #undef Linear_Inline
//
//         // ==============触发重绘=============
//         uint16_t page = get_page(y);// 起始页
//         for (uint16_t i=0;i<GUI_PAGE_HEIGHT(y, height+2);++i)
//         {
//             update_dirty_col(page,x,x+width+1);
//             ++page;
//         }
     }
}


#endif //GUI_WAVEFORM_HPP
