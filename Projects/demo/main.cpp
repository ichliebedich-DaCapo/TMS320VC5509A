#include<app_conf.h>
#include <zq_timer.h>
#include<zq_interrupt.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>
#include<gui.h>
#include <AsyncDelay.h>
#include<Flag.h>

extern void ui_handler();




int main()
{
    ZQ_Init();
    lcd_init();
    GUI_Init();


    lcd_clear();


    for (;;)
    {
        ui_handler();
        GUI_Render::handler<oled_write_data>(); // 处理GUI事件
    }
}
#endif
