#include <iostream>
#include <SDL2/SDL.h>
#include "simulator.hpp"
#include "gui.h"

extern int keyboard_thread(void *data);

// 宏
#define MEASURE_ENABLE 0 // 是否测量性能

// 界面函数
extern void ui_handler(); // 界面处理（动态）

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
#if MEASURE_ENABLE
    SDL_CreateThread(tick_handler, "tick", nullptr);
#endif

    GUI_Init();

    // 主循环
    while (simulator_is_running())
    {
        simulator_event_Handler(); // 模拟器事件处理
#if MEASURE_ENABLE
        last_tick[tick_index] = tick;
#endif

        // --------------测量开始-------------

        ui_handler();

#if GUI_PAGE_MODE==8
        GUI_Render::handler<oled_write_data>(); // 界面处理（动态）
#else
        GUI_Render::handler<oled_write_data,oled_write_data>();// 界面处理（动态）
#endif

        // --------------测量结束-------------
#if MEASURE_ENABLE
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

#endif
        SDL_Delay(5); // 短暂休眠
    }


    // 清理资源
    simulator_quit();
    return 0;
}
