#include <zq_drivers.h>
#include <zq_gui.h>
#include <core.h>

int main()
{
    ZQ_Init(); // 系统初始化
    GUI::Render::init<oled_init>(); // GUI初始化
    GUI::Flag::partialUpdate::set(); // 分页刷新
    Core::init(); // 核心逻辑初始化
    for (;;)
    {
        Core::handler(); // 核心逻辑
        // GUI::Render::handler<oled_write_data>(); // 处理GUI事件
    }
}

//---------------------------------中断处理函数--------------------------------------//
extern "C"
{

void interrupt Timer1_ISR()
{
    ISR::timer1();
}
}
