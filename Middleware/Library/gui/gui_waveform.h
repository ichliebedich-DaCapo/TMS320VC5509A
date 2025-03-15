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
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
class WaveformView : public GUI_Object
{
public:
    void create();

protected:
    // 绘制静态网格函数

    // 绘制波形函数
    static void draw();

private:
    static uint16_t buffer[MAX_POINT_NUM]; // 存储波形数据
    static uint16_t static_buffer[width * GUI_PAGE_HEIGHT(height)]; // 存储静态网格数据，由于一页8位，高度需要除以8,向上取整
    static uint16_t index; // 波形数据索引
};

// =======================变量====================
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height>::buffer[MAX_POINT_NUM] = {}; // 存储波形数据
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height>::static_buffer[width * GUI_PAGE_HEIGHT(height)] = {};
// 存储静态网格数据
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
uint16_t WaveformView<MAX_POINT_NUM, x, y, width, height>::index; // 波形数据索引

// ==================================模板类的实现==========================================
// 创建函数
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
void WaveformView<MAX_POINT_NUM, x, y, width, height>::create()
{
    GUI_Object::create(draw); // 里面可以塞入一个指针，然后把当前的draw保存到对象数组里

    // 把静态网格的内容保存到缓冲区里
    draw_hline<0,width-1,0,static_buffer,width,height>();
    buffer_copy<x, y, static_buffer, width, height>();
    // 再绘制波形
}

// 绘制波形函数
template<uint16_t MAX_POINT_NUM, uint16_t x, uint16_t y, uint16_t width, uint16_t height>
void WaveformView<MAX_POINT_NUM, x, y, width, height>::draw()
{
    // 绘制静态网格
    // 绘制波形
}


#endif //GUI_WAVEFORM_HPP
