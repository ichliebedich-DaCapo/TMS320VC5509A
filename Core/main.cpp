
#include <zq_gui.h>
#include <zq_timer.h>
#include<zq_interrupt.h>
#include <zq_init.h>

#include<Flag.h>
#include <led.h>
#include <zq_systick.h>

int main()
{
    ZQ_Init();
    GUI::Render::init<oled_init>();
    GUI::Flag::partialUpdate::set();

    bsp::led::LED::clear(); // 熄灭LED

    zq::systick::AsyncDelay testDelay;
        testDelay.start(500);

    for (;;)
    {
        // zq::systick::Delay::ms(500);
        if (testDelay.is_timeout())
        {
            bsp::led::LED::toggle(bsp::led::pin::LED_2);
        }

        GUI::Render::handler<oled_write_data>(); // 处理GUI事件
    }
}

//---------------------------------中断处理函数--------------------------------------//

extern "C"
{
void interrupt Timer1_ISR() {}
}


