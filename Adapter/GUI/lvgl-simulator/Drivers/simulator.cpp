//
// Created by fairy on 2025/1/1 15:52.
//
#include "simulator.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

static volatile bool keep_running = true;

constexpr int HOR = 128; // 屏幕宽度
constexpr int VER = 64; // 屏幕高度
constexpr int REAL_HOR = HOR*4;
constexpr int REAL_VER = VER*4;


static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;


// 屏幕显存
static uint16_t TFT_GRAM[REAL_VER][REAL_HOR];
static uint16_t *GRAM = &TFT_GRAM[0][0];

// 触摸屏相关变量
static bool press_state = false;
static int32_t last_x = 0;
static int32_t last_y = 0;

/***********************************内部函数**************************************/
void mouse_handler(SDL_Event *event);


/*******************************模拟器接口************************************/
uint8_t simulator_is_running() { return keep_running; }


/**
 * @brief 初始化 SDL
 * @note 初始化了 SDL 窗口、渲染器、纹理，并创建了一个更新纹理的线程.默认颜色编码为 RGB565
 */
void simulator_init()
{
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        keep_running = false;
        return;
    }

    // 创建窗口
    window = SDL_CreateWindow("LVGL Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              REAL_HOR, REAL_VER, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);

    // 创建渲染器（此处使用硬件加速，可以使用软件加速SDL_RENDERER_SOFTWARE）
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // 创建纹理
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING,
                                REAL_HOR, REAL_VER);


    if (!window || !renderer || !texture)
    {
        std::cerr << "Failed to create window/renderer/texture: " << SDL_GetError() << std::endl;
        SDL_Quit();
        keep_running = false;
        return;
    }

    // 创建更新纹理的线程
    //    SDL_CreateThread([](void *)
    //                     {
    //                         while (keep_running)
    //                         {
    //
    //                             SDL_Delay(10);
    //                         }
    //                         return 1;
    //                     }, "updateSDLGram", nullptr);
}


/**
 * @brief 退出 SDL
 */
void simulator_quit()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


/**
 * @brief 处理 SDL 事件
 */
void simulator_event_Handler()
{
    // 处理SDL事件
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            printf("Key Press\n");
        }

        if (event.type == SDL_KEYUP)
        {
            printf("Key Up\n");
        }

        // 处理鼠标事件
        mouse_handler(&event);

        // 处理其他事件
        switch (event.type)
        {
            case SDL_QUIT:
                keep_running = false;
                break;

            case SDL_KEYDOWN:
                // 失效了，没有任何用
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_TAKE_FOCUS:
                    case SDL_WINDOWEVENT_EXPOSED:

                    default:
                        break;
                }
            default:
                break;
        }
    }

    // 更新纹理
    SDL_UpdateTexture(texture, nullptr, GRAM, REAL_HOR * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}



/***********************************LCD驱动接口******************************************/
//
/**
 * 设置单个像素点
 * @param x 横坐标
 * @param y 纵坐标
 * @param color 颜色，0：白色 1：黑色
 */
static void lcd_write_pixel(const uint16_t x, const uint16_t y, const uint8_t color)
{
    // 填充4x4像素块
    for (int dy = 0; dy < 4; ++dy)
    {
        for (int dx = 0; dx < 4; ++dx)
        {
            // 计算实际显存坐标
            const uint16_t ty = y + dy;
            const uint16_t tx = x + dx;

            // TFT_GRAM[y][x] = is_black ? 0xF000 : 0x6789;// 左黑右白
            TFT_GRAM[ty][tx] = color ? 0x0000 : 0xFFFF;// 左黑右白

        }
    }
}

/**
 * 绘制竖着的一页数据（一页数据是8个像素点）
 * @param page 页 0到7
 * @param column 列 0到127
 * @param data 一页的数据，为1时即黑
 */
void lcd_write_data(const uint16_t page, const uint16_t column, const uint8_t data)
{
    const uint16_t logical_x = column; // 逻辑列坐标
    const uint16_t screen_x_start = logical_x * 4; // 显存起始列坐标

    for (int i = 0; i < 8; ++i) // 遍历8个垂直像素位
    {
        const uint16_t logical_y = page * 8 + i; // 逻辑行坐标
        const uint16_t screen_y_start = logical_y * 4; // 显存起始行坐标
        const bool is_black = data & (1 << i); // 获取当前bit值

        // 填充4x4像素块
        lcd_write_pixel(screen_x_start, screen_y_start, is_black);
    }
}

void lcd_full_flush(const uint8_t *buf)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        // 遍历所有页（8页）
        for (uint8_t col = 0; col <128; col++)
        {
            lcd_write_data(page, col, buf[page * 128 + col]); // 写入LCD
        }
    }
}

void LCD_Clear()
{
    for (int i = 0; i < HOR * VER; ++i)
        GRAM[i] = 0xFFFF;
}


/***********************************触摸屏驱动接口******************************************/
int32_t touchpad_read_xy(int32_t *x, int32_t *y)
{
    // 读取触摸屏数据
    *x = last_x;
    *y = last_y;

    // 返回是否触摸
    return press_state;
}

void mouse_handler(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT)
                press_state = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                press_state = true;
                last_x = event->motion.x;
                last_y = event->motion.y;
            }
            break;
        case SDL_MOUSEMOTION:
            last_x = event->motion.x;
            last_y = event->motion.y;
            break;

        case SDL_FINGERUP:
            press_state = false;
            last_x = (int) ((float) REAL_HOR * event->tfinger.x);
            last_y = (int) ((float) REAL_VER * event->tfinger.y);
            break;
        case SDL_FINGERDOWN:
            press_state = true;
            last_x = (int) ((float) REAL_HOR * event->tfinger.x);
            last_y = (int) ((float) REAL_VER * event->tfinger.y);
            break;
        case SDL_FINGERMOTION:
            last_x = (int) ((float) REAL_HOR * event->tfinger.x);
            last_y = (int) ((float) REAL_VER * event->tfinger.y);
            break;
        default: break;
    }
}


/**************************键盘*****************************/

