#include<app_conf.h>
#include <zq_gui.h>
#include <zq_timer.h>
#include<zq_interrupt.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>

#include <AsyncDelay.h>
#include<Flag.h>
#include <led.h>
#include <zq_systick.h>

int main()
{
    ZQ_Init();
    GUI::Render::init<oled_init>();
    GUI::Flag::updateMode::set();

    bsp::led::LED::clear(); // 熄灭LED


    for (;;)
    {
        zq::systick::Delay::ms(500);
        bsp::led::LED::toggle(bsp::led::pin::LED_2);
        GUI::Render::handler<oled_write_data>(); // 处理GUI事件
    }
}

//---------------------------------子程序--------------------------------------//

extern "C"
{
void interrupt Timer1_ISR() {}
}

#endif
