//
// Created by fairy on 2025/3/3 01:09.
//

#ifndef SIMULATOR
#include <oled.h>
#endif
#include "gui.h"
extern void ui_init();



void GUI_Init()
{
#ifndef SIMULATOR
    oled_init();
#endif
    GUI_Render::clear();// 清屏
    ui_init();
}


