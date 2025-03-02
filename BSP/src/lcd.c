//
// Created by fairy on 2025/2/28 00:13.
//
#include "lcd.h"
// 变量
uint8_t lcd_buffer[8][128];
uint8_t lcd_page_dirty[8];


// 函数
// 初始化屏幕
void lcd_init()
{
    LCD_CTR_GR = 0;
    LCD_CTR_GR = 0x80;
    LCD_CTR_GR = 0;
    LCD_CTR_LR = 0;
    LCD_CTR_LR = 0x40;
    LCD_CTR_LR = 0x0c0;
    lcd_write_cmd(LCD_CMD_TURN_OFF);
}




/**
 * 刷新所有脏页到硬件
 */
void lcd_refresh()
{
    uint8_t page = 0;
    for (page = 0; page < 8; ++page)
    {
        // ----------清除单页---------
        uint8_t x;
        if (!lcd_page_dirty[page]) return; // 跳过未修改页

        // 左半屏 (0~63)
        lcd_set_page(page);
        lcd_write_cmd(LCD_CMD_VER_ADDRESS); // 列地址归零

        for ( x = 0; x < 64; ++x)
        {
            lcd_write_data_left(lcd_buffer[page][x]);
        }

        // 右半屏 (64~127)
        lcd_set_page(page);
        lcd_write_cmd(LCD_CMD_VER_ADDRESS); // 列地址归零
        for (x = 64; x < 128; ++x)
        {
            lcd_write_data_right(lcd_buffer[page][x]);
        }

        lcd_page_dirty[page] = 0; // 清除脏标记
    }
}