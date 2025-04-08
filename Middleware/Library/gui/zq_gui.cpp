//
// Created by fairy on 2025/3/13 10:55.
//
#include "zq_gui.h"
#ifndef SIMULATOR
#include <oled.h>
#endif

namespace GUI
{
    // 变量定义
    uint16_t Base::buffer[GUI_PAGE][GUI_HOR];

    uint16_t FlagBase::flag = 0;

}


namespace GUI
{
    void init()
    {
#ifndef SIMULATOR
        oled_init();
#endif
        Tools::clear();// 清屏
        Render::init();// 初始化渲染器
        Flag::render::set();// 设置渲染标志位
    }
}

