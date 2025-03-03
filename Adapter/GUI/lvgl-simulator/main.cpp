#include <iostream>
#include <SDL2/SDL.h>
#include "simulator.hpp"
#include "u8g2.h"
#include "GUI.h"
#include<ui.h>

extern int keyboard_thread(void *data);

// 为了解决错误：undefined reference to `WinMain'
#undef main
int main(int argc, char *argv[])
{
    // GUI初始化
    simulator_init();

    GUI_Init();
    ui_init();

    // SDL_CreateThread(keyboard_thread, "keyboard", nullptr);// 键盘线程

    // 主循环
    while (simulator_is_running())
    {
        simulator_event_Handler();// 模拟器事件处理

        u8g2_ClearBuffer(&u8g2);

        lcd_refresh(&u8g2);  // 手动刷新到硬件

        SDL_Delay(10);// 短暂休眠
    }

    // 清理资源
    simulator_quit();
    return 0;
}


