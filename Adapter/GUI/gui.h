//
// Created by fairy on 2025/3/3 01:09.
//
#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif
#ifndef SIMULATOR
#include <zq_conf.h>
#endif
#include<zq_gui.h>

void GUI_Init(); // 初始化GUI
#define GUI_handler() gui_handler()// GUI处理函数

#ifdef __cplusplus
}
#endif
#endif //GUI_H
