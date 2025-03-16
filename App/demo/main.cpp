#include<app_conf.h>
#if PROGRAM_INDEX == 0
#include<zq_gpio.h>
#include<gui.h>


extern void ui_handler();

#endif


int main()
{
    GUI_Init();
    using namespace zq::gpio;




    for (;;)
    {
        ui_handler();
        GUI_Render::handler<oled_write_data>(); // 处理GUI事件
    }
}
