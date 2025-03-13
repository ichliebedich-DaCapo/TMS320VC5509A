//
// Created by fairy on 2025/2/28 00:13.
//
#ifndef DEMO_LCD_H
#define DEMO_LCD_H

#ifdef __cplusplus
extern "C" {
#endif
#include<zq_conf.h>

/*寄存器映射*/
#define LCD_CTRL (*(volatile uint32_t *)0x600801)// 如果要写入数据或者命令，那么该寄存器需要置零
#define LCD_CMD (*(volatile uint32_t *)0x602800)// 命令寄存器
#define LCD_DATA_BASE 0x602801
#define LCD_DATA_L (*(volatile uint32_t *)0x602801)  // 左侧的显示屏
#define LCD_DATA_R (*(volatile uint32_t *)0x600802)
#define LCD_DATA(column) (*(volatile uint32_t *)(0x600802 + (( ( (column) & 0x40 ) << 7 ) - ((column) >> 6 & 1) )))// 动态计算列地址


#define LCD_CTR_GR (*(volatile uint32_t *)0x600800)
#define LCD_CTR_KEY (*(volatile uint32_t *)0x602800)
#define LCD_CTR_LR (*(volatile uint32_t *)0x602803)

/*寄存器命令*/
#define LCD_CMD_TURN_ON 0x3f
#define LCD_CMD_TURN_OFF 0x3e
#define LCD_CMD_START_LINE 0xc0 // 设置显示起始行 0xc0 + (0~63)
#define LCD_CMD_PAGE 0xb8   // 设置操作页 0xb8 + (0~7)
#define LCD_CMD_VER_ADDRESS 0x40    // 设置操作列 0x40 + (0~63)

/*标志*/

/*变量*/
// 显示缓冲区：8页 x 128列，每个字节存储一列的8行数据
extern uint8_t lcd_buffer[8][128];
// 标记页是否被修改，减少刷新范围
extern uint8_t lcd_page_dirty[8];

/*====================================底层基础操作=========================================*/
//
/**
 * 向左侧屏幕写入数据
 * @param data 数据
 */
INLINE void lcd_write_data_left(const uint8_t data)
{
    LCD_DATA_L = data;
    LCD_CTRL = 0;
}

/**
 * 向右侧屏幕写入数据
 * @param data 数据
 */
INLINE void lcd_write_data_right(const uint8_t data)
{
    LCD_DATA_R = data;
    LCD_CTRL = 0;
}

/**
 * 写入命令
 * @param cmd 命令
 */
INLINE void lcd_write_cmd(const uint8_t cmd)
{
    LCD_CMD = cmd;
    LCD_CTRL = 0;
}

/**
* 设置当前行(页0~7)
*/
INLINE void lcd_set_page(const uint8_t page)
{
    lcd_write_cmd(LCD_CMD_PAGE + (page & 0x07)); // 限制 page 在 0~7
}

// 设置当前列 (0~63)
INLINE void lcd_set_column(const uint8_t column)
{
    lcd_write_cmd(LCD_CMD_VER_ADDRESS + column); // 左半屏
}


// 清屏
#define lcd_clear()\
{\
    int i, j;\
    lcd_write_cmd(LCD_CMD_START_LINE);\
    for (i = 0; i < 8; ++i)\
    {\
        lcd_set_page(i);\
        lcd_set_column(0);\
        for (j = 0; j < 64; ++j)\
            lcd_write_data_left(0);\
        lcd_set_page(i);\
        lcd_set_column(0);\
        for (j = 0; j < 64; ++j)\
            lcd_write_data_right(0);\
    }\
}

INLINE void lcd_write_data(const uint16_t page, const uint16_t column, const uint8_t data)
{
    lcd_set_page(page);
    if (column > 63)
    {
        lcd_set_column(column - 64);
        lcd_write_data_right(data);
    }
    else
    {
        lcd_set_column(column);
        lcd_write_data_left(data);
    }
}

// 更高效的写入语句,不会刷新流水线，从而避免分支惩罚
INLINE void lcd_write_data_fast(const uint16_t page, const uint16_t column, const uint8_t data)
{
    lcd_set_page(page);
    lcd_set_column(column & 0x3f);
    LCD_DATA(column) = data;
    LCD_CTRL = 0;
}

// 理论上更高效的写入语句，但是需要测试！！
// buf: 128字节
INLINE void lcd_flush(const uint8_t *buf)
{
    // int i, j;
    // lcd_write_cmd(LCD_CMD_START_LINE);
    // for (i = 0; i < 8; ++i)
    // {
    //     // 左半边屏幕
    //     lcd_set_page(i);
    //     lcd_set_column(0);
    //     for (j = 0; j < 64; ++j)
    //         lcd_write_data_left(buf[i<<7+j]);
    //
    //     // 右半边屏幕
    //     lcd_set_page(i);
    //     lcd_set_column(0);
    //     for (j = 0; j < 64; ++j)
    //         lcd_write_data_right(buf[i<<7+j+64]);
    // }

    int16_t i, j;
    lcd_write_cmd(LCD_CMD_START_LINE);
    for (i = 0; i < 8; ++i)
    {
        lcd_set_page(i);
        lcd_set_column(0); // 设置一次列地址

        // 合并左右屏写入
        for (j = 0; j < 64; ++j)
        {
            lcd_write_data_left(buf[(i << 7) + j]); // 左屏
            lcd_write_data_right(buf[(i << 7) + j + 64]); // 右屏
        }
    }
}

/*====================================缓冲区函数=========================================*/
/**
 * 设置单点像素（缓冲区版，无硬件操作）
 * @param x 列坐标 (0~127)
 * @param y 行坐标 (0~63)
 * @param data 像素颜色，0：熄灭，1：点亮
 */
INLINE void lcd_write_pixel(const uint8_t x, const uint8_t y, const uint8_t data)
{
    // 计算页和位位置（无除法/取余）
    const uint8_t page = y >> 3; // y / 8 → y >> 3
    const uint8_t bit_pos = 7 - (y & 0x7); // y % 8 → y & 0x7

    // 修改缓冲区
    if (data)
    {
        lcd_buffer[page][x] |= (1 << bit_pos);
    }
    else
    {
        lcd_buffer[page][x] &= ~(1 << bit_pos);
    }

    // 标记页为脏
    lcd_page_dirty[page] = 1;
}

// 填充矩形区域（缓冲区版）
INLINE void lcd_fill_rect(const uint8_t x_start, const uint8_t x_end, const uint8_t y_start, const uint8_t y_end)
{
    uint8_t x, y;
    for (y = y_start; y <= y_end; ++y)
    {
        const uint8_t page = y >> 3;
        const uint8_t bit_mask = 1 << (7 - (y & 0x7));
        for (x = x_start; x <= x_end; ++x)
        {
            lcd_buffer[page][x] |= bit_mask;
        }
        lcd_page_dirty[page] = 1;
    }
}

/*====================================操作=========================================*/
//
void lcd_init();

void lcd_refresh();


#ifdef __cplusplus
}
#endif
#endif //DEMO_LCD_H
