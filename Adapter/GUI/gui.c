//
// Created by fairy on 2025/3/3 01:09.
//
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
