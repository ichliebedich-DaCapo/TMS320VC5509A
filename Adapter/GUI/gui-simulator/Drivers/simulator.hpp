//
// Created by fairy on 2025/1/1 15:52.
//
#ifndef LVGL_SIMULATOR_SIMULATOR_HPP
#define LVGL_SIMULATOR_SIMULATOR_HPP

#ifdef __cplusplus
#include<cstdint>
#endif

/*****************/
#ifdef __cplusplus
extern "C" {
#endif


void simulator_init();

void simulator_quit();

void simulator_event_Handler();

uint8_t simulator_is_running();

/********************LCD驱动接口***********************/
void oled_write_data(uint16_t page,uint16_t start_col,uint16_t end_col, const uint16_t *buf);


void LCD_Clear();

/**********************触摸屏驱动接口**********************/
int32_t touchpad_read_xy(int32_t *x, int32_t *y);


#ifdef __cplusplus
}
#endif


#endif //LVGL_SIMULATOR_SIMULATOR_HPP
