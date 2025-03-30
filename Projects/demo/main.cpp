#include<app_conf.h>
#include <zq_timer.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>
#include<gui.h>


extern void ui_handler();

#endif


int main()
{
    ZQ_Init();
    GUI_Init();

    zq::timer::TIM0::init(TIM_FREQ_200M_to_1M);





    for (;;)
    {
        ui_handler();
        GUI_Render::handler<oled_write_data>(); // 处理GUI事件
    }
}
