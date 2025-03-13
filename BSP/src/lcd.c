//
// Created by fairy on 2025/2/28 00:13.
//
#include "lcd.h"



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




