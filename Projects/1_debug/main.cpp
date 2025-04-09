//
// Created by fairy on 2025/3/1 21:22.
//
//-------------------------------相关头文件------------------------------------//
#include<app_conf.h>
#include <zq_conf.h>
#include <zq_pll.h>
#include<zq_gpio.h>
#include <zq_init.h>
#include <zq_interrupt.h>
#include<zq_timer.h>
#if PROGRAM_INDEX == 1
#include <stdio.h>
#include <zq_adc.h>
#include<led.h>
#include <dip.h>
#include<zq_gui.h>
#include<oled.h>
#include<zq_gui.h>
//-------------------------------声明----------------------------------//
#define BUF_SIZE 0x64
#define TRUE 1
#define FALSE 0

struct PARMS
{
    int Beta;
    int EchoPower;
    int ErrorPower;
    int Ratio;
    struct PARMS *Link;
};

//-------------------------------工作变量定义----------------------------------//
int inp_buffer[BUF_SIZE]; // 输入缓冲区
int out_buffer[BUF_SIZE]; // 输出缓冲区
int *input;
int *output;

int volume = 2;

// 没有别的作用，只是用于在变量窗口中查看
struct PARMS str =
{
    2934, 9432, 213, 9432, &str
};

//-------------------------------调用子程序规则--------------------------------//
int read_signals(int *input);

int write_buffer(const int *input, int *output, int count);

int output_signals(int *output);


//-----------------------------------主程序------------------------------------//
uint32_t count =0;
int main()
{
    ZQ_Init();

    oled_init();
    oled_clear();
    bsp::LED::clear();

    zq::timer::Timer0::init(TIM_FREQ_200M_to_100K);
zq::isr::start_timer();

    // ======初始化======
    int i = 0;
    input = inp_buffer;
    output = out_buffer;



    // ======无限循环======
    while (TRUE)
    {
        // 如果如我预期是200MHz，那么count应该接近
        // ++count;
        static uint16_t temp =0;

        // count = zq::timer::TIM<0>::read();
        if (count >= 20000)// 5000000
        {
            count = 0;
            bsp::LED::toggle(bsp::led::pin::LED_1);
            // temp = ~temp;
            // if (temp)
            //     zq::gpio::GPIO_Normal0::high();
            // else
            //     zq::gpio::GPIO_Normal0::low();
        }


        static uint16_t tim = zq::timer::TIM<0>::read();
        static uint16_t psc = zq::timer::PRSC<0>::PSC::read_bits();



//  拨码开关
        // bsp::LED::set(bsp::DIP::get());
        // bsp::LED::set(zq::gpio::GPIO_Normal2::read());
    }
}

//---------------------------------子程序--------------------------------------//

extern "C"
{
interrupt void TimerISR()
{
    count ++;
    bsp::LED::toggle(bsp::led::pin::LED_3);
}
}

#endif
