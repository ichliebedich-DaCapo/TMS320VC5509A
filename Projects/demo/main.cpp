#include<app_conf.h>
#include <zq_gui.h>
#include <zq_timer.h>
#include<zq_interrupt.h>
#if PROGRAM_INDEX == 0
#include <zq_init.h>

#include <AsyncDelay.h>
#include<Flag.h>
#include <led.h>
static uint16_t count =0;





int main()
{
    ZQ_Init();
    GUI::Render::init<oled_init>();
    GUI::Flag::updateMode::set();

    bsp::led::LED::clear();
    zq::timer::Timer0::init(TIM_FREQ_192M_to_1K);
    zq::timer::Timer0::start_IT();

    for (;;)
    {
        if (count>1000)
        {
            count =0;
            bsp::led::LED::toggle(bsp::led::pin::LED_2);
        }
       GUI::Render::handler<oled_write_data>();// 处理GUI事件
    }
}

//---------------------------------子程序--------------------------------------//
extern "C"
{
    void interrupt Timer0_ISR()
    {
++count;
    }
}


#endif
