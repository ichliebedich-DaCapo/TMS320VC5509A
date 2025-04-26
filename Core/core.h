//
// Created by fairy on 2025/4/26 17:37.
//
#ifndef CORE_H
#define CORE_H
#include <zq_drivers.h>
#include <led.h>
#include <zq_i2c.h>
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
        using namespace zq;
        using namespace bsp;
        gpio::GPIO_Normal0::set_dir(gpio::Dir::Dir_Output);
        testDelay.start(400);
        led::LED::clear(); // 熄灭LED

        i2c::Config cfg;
        cfg.system_clock=192000000;
        cfg.bitrate=400000;
        cfg.loopback=false;
        I2C::init(cfg);



    }


    // ==================== 事件处理 ====================
    static void handler()
    {
        using namespace zq;
        using namespace bsp;


        // 发送数据到温度传感器
        const uint16_t temp_sensor_addr = 0x48;
        const uint16_t cmd = 0x00; // 读取温度命令

        I2C::start();
        I2C::send(temp_sensor_addr << 1);
        I2C::send(cmd);
        I2C::stop();

        if (testDelay.is_timeout())
        {
            led::LED::toggle(led::pin::LED_2);
        }
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
