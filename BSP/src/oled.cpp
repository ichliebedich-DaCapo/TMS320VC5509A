//
// Created by fairy on 2025/2/28 00:13.
//
#include "oled.h"


// 函数
// 初始化屏幕
void oled_init()
{
    LCD_CTR_GR = 0;
    LCD_CTR_GR = 0x80;
    LCD_CTR_GR = 0;
    LCD_CTR_LR = 0;
    LCD_CTR_LR = 0x40;
    LCD_CTR_LR = 0x0c0;
    lcd_write_cmd(LCD_CMD_TURN_OFF);
    delay(10);
    lcd_write_cmd(LCD_CMD_TURN_ON);
}

void oled_clear()
{
    lcd_write_cmd(LCD_CMD_START_LINE);
    for (int i = 0; i < 8; ++i)
    {
        lcd_set_page(i);
        lcd_set_column(0);
        for (int j = 0; j < 64; ++j)
        {
            lcd_write_data_left(0x00);
            lcd_write_data_right(0x00);
        }
    }
}

void oled_write_data(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf) {
    // 参数检查
    if (start_col > end_col || end_col >= 128) {
        return; // 无效参数，直接返回
    }

    // 计算左半屏范围
    const uint16_t left_start = start_col;
    const uint16_t left_end = (end_col < 63) ? end_col : 63;
    const uint16_t left_len = (left_end >= left_start) ? (left_end - left_start + 1) : 0;

    // 计算右半屏范围
    const uint16_t right_start = (start_col > 63) ? start_col : 64;
    const uint16_t right_end = end_col;
    const uint16_t right_len = (right_end >= right_start) ? (right_end - right_start + 1) : 0;

    // 处理左半屏
    if (left_len > 0) {
        lcd_set_page(page);          // 设置页地址
        lcd_set_column(left_start);  // 设置左半屏起始列
        for (uint16_t i = 0; i < left_len; ++i) {
            lcd_write_data_left(buf[i]); // 写入左半屏数据
        }
    }

    // 处理右半屏
    if (right_len > 0) {
        lcd_set_page(page);           // 设置页地址（若页地址全局可省略）
        lcd_set_column(right_start);  // 设置右半屏起始列（内部转换列地址）
        for (uint16_t i = 0; i < right_len; ++i) {
            lcd_write_data_right(buf[left_len + i]); // 写入右半屏数据
        }
    }
}
