#include<app_conf.h>
#include <zq_gui.h>
#include <zq_timer.h>
#include<zq_interrupt.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>

#include <AsyncDelay.h>
#include<Flag.h>

extern void ui_handler();



uint16_t buf[10]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

int main()
{
    ZQ_Init();
    GUI::Render::init<oled_init>();
    for (;;)
    {

       GUI::Render::handler<oled_write_data>();// 处理GUI事件
    }
}

extern "C"
{
    interrupt void TimerISR()
    {
        // count ++;
        // bsp::LED::toggle(bsp::led::pin::LED_3);
    }
}

#endif
