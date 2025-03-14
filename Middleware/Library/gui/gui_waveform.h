//
// Created by fairy on 2025/3/14 23:57.
//
#ifndef GUI_WAVEFORM_HPP
#define GUI_WAVEFORM_HPP
#include"zq_gui.h"

/**
 * 示波器组件
 * @tparam MAX_POINT_NUM 波形的点数
 * @note 默认是滚动更新，当然，其他模式其实根本没做
 */
template<uint16_t MAX_POINT_NUM>
class WaveformView : public GUI_Object
{
public:
    void create(coord_t x, coord_t y, coord_t width, coord_t height);
protected:
    // 坐标处理
    void set_x(const uint16_t x){coord&=~(0x00FF);coord|=x;}
    void set_y(const uint16_t y){coord&=~(0xFF00);coord|=y<<8;}
    void set_coord(const uint16_t x,const uint16_t y){coord=x|y<<8;}
    uint16_t get_x() const {return coord&0x00FF;}
    uint16_t get_y() const {return coord>>8;}
    // 宽高处理
    void set_width_height(const uint16_t width,const uint16_t height){width_height=width|height<<8;}
    uint16_t get_width() const {return width_height&0x00FF;}
    uint16_t get_height() const {return width_height>>8;}
    // 绘制静态网格函数
    // 绘制波形函数
private:
    uint16_t buffer[MAX_POINT_NUM] = {};// 存储波形数据
    uint16_t coord = 0;// 坐标，包含了横坐标和纵坐标
    uint16_t width_height = 0;// 宽高
    uint16_t index = 0;// 波形数据索引
};

template<uint16_t MAX_POINT_NUM>
void WaveformView<MAX_POINT_NUM>::create(const coord_t x, const coord_t y, coord_t width, coord_t height)
{
    GUI_Object::create();// 里面可以塞入一个指针，然后把当前的draw保存到对象数组里
    set_coord(x,y);
    set_width_height(width,height);

    // 把静态网格的内容保存到缓冲区里
    // 再绘制波形
}




#endif //GUI_WAVEFORM_HPP
