//
// Created by fairy on 2025/3/13 10:55.
//
#include "zq_gui.h"
// 变量
uint8_t lcd_buffer[8][128];
// 脏页跟踪（每个页独立记录修改范围）
PageDirtyInfo lcd_dirty_info[8] = {
    {0, 128, 0}, {0, 128, 0}, {0, 128, 0}, {0, 128, 0},
    {0, 128, 0}, {0, 128, 0}, {0, 128, 0}, {0, 128, 0}
};

// 函数
/**
 * 刷新所有脏页到硬件
 */
void gui_refresh_buf()
{
#ifdef SIMULATOR

    uint8_t page, x;
    for (page = 0; page < 8; page++)
    {
        PageDirtyInfo *p = &lcd_dirty_info[page];
        if (!p->is_dirty || p->min_col > p->max_col) continue;

        lcd_write_page(page,lcd_buffer);

        // 重置脏标记
        p->is_dirty = 0;
        p->min_col = 128;
        p->max_col = 0;
    }
#else
    uint8_t page, x;
    for (page = 0; page < 8; page++)
    {
        PageDirtyInfo *p = &lcd_dirty_info[page];
        if (!p->is_dirty || p->min_col > p->max_col) continue;

        // 硬件刷新优化（仅发送变更区域）
        lcd_set_page(page);

        // 左半屏处理（0-63）
        if (p->min_col < 64)
        {
            const uint8_t start = p->min_col;
            const uint8_t end = (p->max_col < 64) ? p->max_col : 63;
            lcd_set_column(start);
            for (x = start; x <= end; x++)
            {
                lcd_write_data_left(lcd_buffer[page][x]);
            }
        }

        // 右半屏处理（64-127）
        if (p->max_col >= 64)
        {
            const uint8_t start = (p->min_col > 64) ? p->min_col : 64;
            const uint8_t end = p->max_col;
            lcd_set_column(start - 64);
            for (x = start; x <= end; x++)
            {
                lcd_write_data_right(lcd_buffer[page][x]);
            }
        }

        // 重置脏标记
        p->is_dirty = 0;
        p->min_col = 128;
        p->max_col = 0;
    }
#endif
}
