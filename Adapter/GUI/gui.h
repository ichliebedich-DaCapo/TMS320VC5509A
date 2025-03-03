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
    #include <u8g2.h>
extern u8g2_t u8g2;

    void lcd_refresh(u8g2_t *u8g2);
    void GUI_Init();

#ifdef __cplusplus
}
#endif
#endif //GUI_H