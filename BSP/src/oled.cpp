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
            lcd_write_data_left(0xF0);
        lcd_set_page(i);
        lcd_set_column(0);
        for (int j = 0; j < 64; ++j)
            lcd_write_data_right(0x0F);
    }
}

void oled_write_data(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf)
{
    lcd_set_page(page);
    lcd_set_column(start_col & 0x3f);
    for (int16_t col = start_col; col <= end_col; ++col)
    {
        // 写入LCD
        // LCD_DATA(col) = buf[col - start_col];
        lcd_write_data_left(buf[col - start_col]);
        LCD_CTRL = 0;
    }
}
