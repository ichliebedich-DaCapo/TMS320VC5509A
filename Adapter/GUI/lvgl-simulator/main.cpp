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
    // SDL_CreateThread(keyboard_thread, "keyboard", nullptr);// 键盘线程

    GUI_Init();

    ui_init();


    static int i = 0;
    // 主循环
    while (simulator_is_running())
    {
        simulator_event_Handler();// 模拟器事件处理

        i++;
        if (i==100)
        {
            i=0;
            u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);  // 绘制空心矩形

        }

        u8g2_NextPage(&u8g2);
        SDL_Delay(10);// 短暂休眠
    }

    // 清理资源
    simulator_quit();
    return 0;
}


