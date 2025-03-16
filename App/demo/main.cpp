#include<app_conf.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>
#include<gui.h>


extern void ui_handler();

#endif


int main()
{
    ZQ_Init();
    GUI_Init();





    for (;;)
    {
        ui_handler();
        GUI_Render::handler<oled_write_data>(); // 处理GUI事件
    }
}
