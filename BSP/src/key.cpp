//
// Created by fairy on 2025/4/26 22:47.
//
#include <key.h>
// 变量
uint16_t bsp::Key::code=0;
uint16_t bsp::Key::sign=0;
#include <zq_systick.h>
#include <zq_delay.h>
#include <zq_interrupt.h>
// 函数
volatile uint16_t test;
#define CTRKEY (*(unsigned int *)0x602800)		//       ݻض  Ĵ
#define CTRCLKEY (*(unsigned int *)0x600801)	//      ̼Ĵ
extern "C" void EXINT2_ISR()
{
    bsp::Key::setCode(CTRKEY);
    delay(200);
    if(bsp::Key::getCode()!=0)
    {
        bsp::Key::setCode(CTRKEY);
        test=CTRCLKEY;
        bsp::Key::setSign();
    }

     // delay(15000);

}