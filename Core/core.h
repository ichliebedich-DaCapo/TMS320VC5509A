//
// Created by fairy on 2025/4/26 17:37.
//
#ifndef CORE_H
#define CORE_H
#include <zq_drivers.h>
#include <led.h>
/**
 * @brief 核心逻辑类
 * @note 为了把核心业务逻辑与main分离，定义了这个类，同时为了避免调用开销，只得写在头文件里。
 */
class Core
{
    static zq::systick::AsyncDelay testDelay;
public:
    // ==================== 初始化 ====================
    static void init()
    {
        zq::gpio::GPIO_Normal0::set_dir(zq::gpio::Dir::Dir_Output);


        testDelay.start(500);
        bsp::led::LED::clear(); // 熄灭LED
    }


    // ==================== 事件处理 ====================
    static void handler()
    {
        zq::systick::Delay::us(50);
        zq::gpio::GPIO_Normal0::toggle();

        // if (testDelay.is_timeout())
        // {
        //     bsp::led::LED::toggle(bsp::led::pin::LED_2);
        // }
    }
private:
};


// ============================== 中断处理类 ==============================
class ISR
{
public:
    static void timer1()
    {

    }
};

#endif //CORE_H
