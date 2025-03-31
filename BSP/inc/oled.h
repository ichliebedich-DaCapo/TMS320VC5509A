//
// Created by fairy on 2025/2/28 00:13.
//
#ifndef DEMO_LCD_H
#define DEMO_LCD_H
#include<zq_conf.h>

// 动态计算列地址
#define LCD_DATA(column) (*(volatile uint32_t *)(0x600802 + (( ( (column) & 0x40 ) << 7 ) - ((column) >> 6 & 1) )))

namespace bsp
{
    namespace oled
    {
        DECLARE_EXMEM_REGISTER_T(CTRL, 0x600801); // 如果要写入数据或者命令，那么该寄存器需要置零
        DECLARE_EXMEM_REGISTER_T(CMD, 0x602800); // 命令寄存器
        DECLARE_EXMEM_REGISTER_T(DATA_BASE, 0x602801);

        DECLARE_EXMEM_REGISTER_T(DATA_L, 0x602801); // 左侧的显示屏
        DECLARE_EXMEM_REGISTER_T(DATA_R, 0x600802); // 右侧的显示屏

        DECLARE_EXMEM_REGISTER_T(CTR_GR, 0x600800);
        DECLARE_EXMEM_REGISTER_T(CTR_KEY, 0x602800);
        DECLARE_EXMEM_REGISTER_T(CTR_LR, 0x602803);

        DECLARE_ATTRIBUTE(OP_CMD,
            ON = 0x3f,
            OFF = 0x3e,
            START_LINE = 0xc0, // 设置显示起始行 0xc0 + (0~63)
            PAGE = 0xb8,   // 设置操作页 0xb8 + (0~7)
            VER_ADDRESS = 0x40    // 设置操作列 0x40 + (0~63)
            );

    }

    class OLED
    {
        // 寄存器
    public:

        /**
         * 向左侧屏幕写入数据
         * @param data 数据
         */
        INLINE void write_data_left(const uint16_t data)
        {
            oled::DATA_L::write(data);
            oled::CTRL::write(0);
        }

        /**
         * 向右侧屏幕写入数据
         * @param data 数据
         */
        INLINE void write_data_right(const uint16_t data)
        {
            oled::DATA_R::write(data);
            oled::CTRL::write(0);
        }

        /**
         * 写入命令
         * @param cmd 命令
         */
        INLINE void write_cmd(const uint16_t cmd)
        {
            oled::CMD::write(cmd);
            oled::CTRL::write(0);
        }

        /**
        * 设置当前行(页0~7)
        */
        INLINE void set_page(const uint16_t page)
        {
            write_cmd(oled::OP_CMD::START_LINE + (page & 0x07)); // 限制 page 在 0~7
        }

        // 设置当前列 (0~63)
        INLINE void set_column(const uint16_t column)
        {
            write_cmd(oled::OP_CMD::VER_ADDRESS + column); // 左半屏
        }

        INLINE void clear()
        {
            int j;
            write_cmd(oled::OP_CMD::START_LINE);
            for (int i = 0; i < 8; ++i)
            {
                set_page(i);
                set_column(0);
                for (j = 0; j < 64; ++j)
                    write_data_left(0);
                set_page(i);
                set_column(0);
                for (j = 0; j < 64; ++j)
                    write_data_right(0);
            }
        }
    };
}

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


/*====================================底层基础操作=========================================*/
//
/**
 * 向左侧屏幕写入数据
 * @param data 数据
 */
INLINE void lcd_write_data_left(const uint16_t data)
{
    LCD_DATA_L = data;
    LCD_CTRL = 0;
}

/**
 * 向右侧屏幕写入数据
 * @param data 数据
 */
INLINE void lcd_write_data_right(const uint16_t data)
{
    LCD_DATA_R = data;
    LCD_CTRL = 0;
}

/**
 * 写入命令
 * @param cmd 命令
 */
INLINE void lcd_write_cmd(const uint16_t cmd)
{
    LCD_CMD = cmd;
    LCD_CTRL = 0;
}

/**
* 设置当前行(页0~7)
*/
INLINE void lcd_set_page(const uint16_t page)
{
    lcd_write_cmd(LCD_CMD_PAGE + (page & 0x07)); // 限制 page 在 0~7
}

// 设置当前列 (0~63)
INLINE void lcd_set_column(const uint16_t column)
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

INLINE void lcd_write_data(const uint16_t page, const uint16_t column, const uint16_t data)
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
INLINE void lcd_write_data_fast(const uint16_t page, const uint16_t column, const uint16_t data)
{
    lcd_set_page(page);
    lcd_set_column(column & 0x3f);
    LCD_DATA(column) = data;
    LCD_CTRL = 0;
}

void oled_write_data(uint16_t page, uint16_t start_col, uint16_t end_col, const uint16_t *buf);


// 理论上更高效的写入语句，但是需要测试！！
// buf: 128字节
INLINE void lcd_flush(const uint16_t *buf)
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


/*====================================操作=========================================*/
//
void lcd_init();

// void lcd_write_page(uint16_t page,uint16_t*buf);


#endif //DEMO_LCD_H
