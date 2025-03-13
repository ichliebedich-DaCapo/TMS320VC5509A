#include<app_conf.h>
#if PROGRAM_INDEX == 0
#include<zq_gpio.h>
#include<gui.h>
#include<demo/ui/ui.h>

extern void ui_init();

extern void ui_handler();

int main()
{
    // ZQ_GPIO_Disable(GPIO_GROUP_NORMAL, GPIO_PIN_0);
    GUI_Init();
    ui_init();

    while (1)
    {
        ui_handler();
    }
}

#endif
