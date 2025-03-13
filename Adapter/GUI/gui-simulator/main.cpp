#include <iostream>
#include <SDL2/SDL.h>
#include "simulator.hpp"
#include<zq_gui.h>
#include "GUI.h"
#include<ui.h>

extern int keyboard_thread(void *data);

// 界面函数
extern "C" {
extern void ui_init(); // 初始静态界面初始化
extern void ui_handler(); // 界面处理（动态）
}

#define TICK_ARRAY_SIZE 20
static uint32_t tick = 0; // 用于测量性能
static uint32_t current_tick[TICK_ARRAY_SIZE];
static uint32_t last_tick[TICK_ARRAY_SIZE];
static uint16_t tick_index = 0;

int tick_handler(void *data)
{
    while (simulator_is_running())
    {
        ++tick;
    }
    return 0;
}

// 为了解决错误：undefined reference to `WinMain'
#undef main
int main(int argc, char *argv[])
{
    // GUI初始化
    simulator_init();
    SDL_CreateThread(keyboard_thread, "keyboard", nullptr); // 键盘线程
    SDL_CreateThread(tick_handler, "tick", nullptr);
    GUI_Init();

    ui_init();

    // 主循环
    while (simulator_is_running())
    {
        simulator_event_Handler(); // 模拟器事件处理
        last_tick[tick_index] = tick;
        // --------------测量开始-------------

        ui_handler(); // 界面处理（动态）

        GUI_handler();

        // --------------测量结束-------------
        current_tick[tick_index] = tick;
        ++tick_index;
        if (tick_index >= TICK_ARRAY_SIZE - 1)
        {
            uint64_t sum_tick = 0;
            for (int i = 0; i < TICK_ARRAY_SIZE; ++i)
                if (current_tick >= last_tick)
                    sum_tick += current_tick[i] - last_tick[i];
                else
                    sum_tick += (0xffffffff - last_tick[i] + current_tick[i]);
            tick_index = 0; // 重置

            // 显示
            const uint32_t average_tick = static_cast<uint32_t>(sum_tick * 1.0f / TICK_ARRAY_SIZE);
            printf("tick:%d,%03d\r\n", average_tick / 1000, average_tick % 1000);
        }


        SDL_Delay(10); // 短暂休眠
    }


    // 清理资源
    simulator_quit();
    return 0;
}
