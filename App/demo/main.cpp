#include<app_conf.h>
#if PROGRAM_INDEX == 0
#include<zq_gpio.h>
#include<gui.h>



extern void ui_handler();

#endif


int main()
{
    GUI_Init();

    for (;;)
    {
        ui_handler();
       GUI_Render::handler(); // 处理GUI事件
    }
}


