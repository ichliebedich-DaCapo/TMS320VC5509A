//
// Created by fairy on 2025/3/3 01:09.
//
#include <stdio.h>
#ifndef SIMULATOR
#include "gui.h"
#include <lcd.h>
#include <u8g2.h>

u8g2_t u8g2; // 全局 U8g2 对象
uint8_t external_buffer[128 * 8]; // 全缓冲模式（128列 x 8页）

// 自定义的 U8g2 回调函数
uint8_t u8g2_custom_hvline(u8g2_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DISPLAY_DRAW_TILE: {
                // 解析像素块数据
                const struct u8x8_tile_struct *tile =arg_ptr;
                const uint8_t x = tile->x_pos;
                const uint8_t y = tile->y_pos;
                const uint8_t *buffer = tile->tile_ptr;

                // 逐列写入数据
                uint8_t col;
                for (col = 0; col < tile->cnt; col++) {
                    lcd_set_page(y);
                    lcd_set_column(x + col);
                    if (y<64)
                        lcd_write_data_left(buffer[col]);
                    else
                        lcd_write_data_right(buffer[col]);
                }
                break;
        }
        default:
            return 0; // 其他消息不处理
    }
    return 1;
}


// GUI初始化，包含了lcd初始化
void GUI_Init()
{
    lcd_init();

    // 配置 U8g2 使用自定义回调
    u8g2_SetupBuffer(&u8g2, external_buffer, 8, u8g2_custom_hvline,&u8g2_cb_r0 );
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0); // 关闭省电模式
}

# 模拟器适配
#else
#include<gui.h>
#include<simulator.hpp>

u8g2_t u8g2; // 全局 U8g2 对象
uint8_t external_buffer[128 * 8]; // 全缓冲区（128列 x 8页）
#define FULL_BUFFER 0   // 全缓冲模式 1:启用 0:禁用


uint8_t u8x8_refresh_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#if FULL_BUFFER ==0
    const struct u8x8_tile_struct *tile = (struct u8x8_tile_struct *) arg_ptr;
    const uint8_t page = tile->y_pos; // 当前页（0-7）
    const uint8_t start_col = tile->x_pos * 8; // 起始列 = Tile位置 x 8
    const uint8_t *data = tile->tile_ptr; // 数据指针
    const uint8_t tile_count = tile->cnt; // 连续Tile数量
    uint8_t col, t;

#endif

    switch (msg)
    {
#if FULL_BUFFER
        case U8X8_MSG_DISPLAY_REFRESH:

            // 静态全刷屏
            lcd_full_flush(external_buffer);
        break;
#endif


        case U8X8_MSG_DISPLAY_DRAW_TILE:
#if FULL_BUFFER ==0
            // 高性能，页面缓冲
            // 遍历所有Tile 16个tile，即16*64

            // 高性能，页面缓冲
            // 遍历所有Tile 16个tile，即16*64
            for (t = 0; t < tile_count; t++)
            {
                // 每个Tile覆盖8列，逐列写入  即一个tile包
                for (col = 0; col < 8; col++)
                {
                    const uint8_t current_col = start_col + t * 8 + col;
                    lcd_write_data(page, current_col, data[t * 8 + col]);
                }
            }
            break;
#endif

        default:
            break;
    }
    return 0;
}

static const u8x8_display_info_t u8x8_custom_128x64_display_info =
{
    /* chip_enable_level = */ 0,
    /* chip_disable_level = */ 1,

    /* post_chip_enable_wait_ns = */ 20,
    /* pre_chip_disable_wait_ns = */ 10,
    /* reset_pulse_width_ms = */ 100, /* SSD1306: 3 us */
    /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
    /* sda_setup_time_ns = */ 50, /* SSD1306: 15ns, but cycle time is 100ns, so use 100/2 */
    /* sck_pulse_width_ns = */ 50,
    /* SSD1306: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
    /* sck_clock_hz = */ 8000000UL,
    /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
    /* spi_mode = */ 0, /* active high, rising edge */
    /* i2c_bus_clock_100kHz = */ 4,
    /* data_setup_time_ns = */ 40,
    /* write_pulse_width_ns = */ 150, /* SSD1306: cycle time is 300ns, so use 300/2 = 150 */
    /* tile_width = */ 16,
    /* tile_height = */ 8,
    /* default_x_offset = */ 0,
    /* flipmode_x_offset = */ 0,
    /* pixel_width = */ 128,
    /* pixel_height = */ 64
};


void GUI_Init()
{
    // 初始化U8g2对象
    u8x8_t *u8x8 = u8g2_GetU8x8(&u8g2);
    u8x8_SetupDefaults(u8x8);
    u8x8->display_cb = u8x8_refresh_cb;
    u8x8->display_info = &u8x8_custom_128x64_display_info;


    // 设置U8g2缓冲区
    u8g2_SetupBuffer(&u8g2, external_buffer, 8, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0); // 关闭省电模式

    // 显示第一页
    u8g2_FirstPage(&u8g2);
}


#endif
