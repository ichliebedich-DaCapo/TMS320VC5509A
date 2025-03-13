//
// Created by fairy on 2025/3/13 10:55.
//
#include "zq_gui.h"

#include <string.h>
// 变量
uint16_t lcd_buffer[8][128];
// 脏页跟踪（每个页独立记录修改范围）
PageDirtyInfo lcd_dirty_info[8] = {
    {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0},
    {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}, {0, GUI_HOR, 0}
};

// 清屏
void gui_clear()
{
    uint16_t page;
    for (page = 0; page < 8; ++page)
    {
        lcd_dirty_info[page].is_dirty = 1;
        lcd_dirty_info[page].min_col = 0;
        lcd_dirty_info[page].max_col = 127;
        memset(lcd_buffer[page], 0, sizeof(lcd_buffer[page]));
    }
}

// 函数
/**
 * 刷新所有脏页到硬件
 */
void gui_refresh_buf()
{
#ifdef SIMULATOR
    uint16_t page;
    for (page = 0; page < 8; page++)
    {
        PageDirtyInfo *p = &lcd_dirty_info[page];
        if (!p->is_dirty || p->min_col > p->max_col) continue;

        lcd_write_page(page,lcd_buffer[page]);

        // 重置脏标记
        p->is_dirty = 0;
        p->min_col = GUI_HOR;
        p->max_col = 0;
    }
#else
    uint16_t page, x;
    for (page = 0; page < 8; page++)
    {
        if (!lcd_dirty_info[page].is_dirty || lcd_dirty_info[page].min_col > lcd_dirty_info[page].max_col) continue;

        // 硬件刷新优化（仅发送变更区域）
        lcd_set_page(page);

        // 左半屏处理（0-63）
        if (lcd_dirty_info[page].min_col < 64)
        {
            const uint16_t start = lcd_dirty_info[page].min_col;
            const uint16_t end = (lcd_dirty_info[page].max_col < 64) ? lcd_dirty_info[page].max_col : 63;
            lcd_set_column(start);
            for (x = start; x <= end; x++)
            {
                lcd_write_data_left(lcd_buffer[page][x]);
            }
        }

        // 右半屏处理（64-127）
        if (lcd_dirty_info[page].max_col >= 64)
        {
            const uint16_t start = (lcd_dirty_info[page].min_col > 64) ? lcd_dirty_info[page].min_col : 64;
            const uint16_t end = lcd_dirty_info[page].max_col;
            lcd_set_column(start - 64);
            for (x = start; x <= end; x++)
            {
                lcd_write_data_right(lcd_buffer[page][x]);
            }
        }

        // 重置脏标记
        lcd_dirty_info[page].is_dirty = 0;
        lcd_dirty_info[page].min_col = 128;
        lcd_dirty_info[page].max_col = 0;
    }
#endif
}
