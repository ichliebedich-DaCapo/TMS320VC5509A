#include<app_conf.h>
#if PROGRAM_INDEX == 0
#include<zq_gpio.h>
#include<gui.h>
#include<demo/ui/ui.h>

#ifdef __cplusplus
extern "C"
{
    extern void ui_handler();
}
#else
extern void ui_handler();
#endif



int main()
{
    // ZQ_GPIO_Disable(GPIO_GROUP_NORMAL, GPIO_PIN_0);
    GUI_Init();

    for (;;)
    {
        ui_handler();
        GUI_handler();// 处理GUI事件
    }
}

#endif
