//
// Created by fairy on 2025/3/30 18:12.
//
#ifndef ZQ_INTERRUPT_H
#define ZQ_INTERRUPT_H
#include<zq_conf.h>
#include<zq_cpu.h>

namespace zq
{
    // 补充相关寄存器的位域
    namespace cpu
    {
        // 中断标志寄存器
        DECLARE_BIT_FIELD(IFR0,DMAC5,15);  // DMA通道5中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,DMAC4,14);  // DMA通道4中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,XINT2,13);  // XINT2外部中断或MMC/SD2中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,RINT2,12);  // McBSP2接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,INT3,11);   // 外部中断3或看门狗定时器中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,DSPINT,10); // HPI主机到DSP的中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,DMAC1,9);   // DMA通道1中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,USB, 8);    // USB中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,XINT1,7);   // XINT1外部中断或MMC/SD1中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,RINT1,6);   // McBSP1接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,RINT0,5);   // McBSP0接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IFR0,TINT0,4);   // 定时器0中断标志位
        DECLARE_BIT_FIELD(IFR0,INT2,3);    // 外部中断2标志位
        DECLARE_BIT_FIELD(IFR0,INT0,2);    // 外部中断1标志位（注意表格中疑似标注错误）
        DECLARE_BITS_FIELD(IFR0,RSVED,2,0); // 位1-0为保留位（0x0002-0x0001），需保持写0


        // 中断使能寄存器
        DECLARE_BIT_FIELD(IER0,DMAC5,15);  // DMA通道5中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,DMAC4,14);  // DMA通道4中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,XINT2,13);  // XINT2外部中断或MMC/SD2中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,RINT2,12);  // McBSP2接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,INT3,11);   // 外部中断3或看门狗定时器中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,DSPINT,10); // HPI主机到DSP的中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,DMAC1,9);   // DMA通道1中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,USB,8);     // USB中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,XINT1,7);   // XINT1外部中断或MMC/SD1中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,RINT1,6);   // McBSP1接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,RINT0,5);   // McBSP0接收中断标志/屏蔽位
        DECLARE_BIT_FIELD(IER0,TINT0,4);   // 定时器0中断标志位
        DECLARE_BIT_FIELD(IER0,INT2,3);    // 外部中断2标志位
        DECLARE_BIT_FIELD(IER0,INT0,2);    // 外部中断1标志位（注意表格中疑似标注错误）
        DECLARE_BIT_FIELD(IER0,RSVED,0);   // 位1-0为保留位（0x0002-0x0001），需保持写0


        DECLARE_BITS_FIELD(IFR1,RSVED, 5, 11);  // [15:11] 保留位（必须写0）
        DECLARE_BIT_FIELD(IFR1,RTOS,10);        // [10]    实时操作系统中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,DLOG,9);         // [9]     数据日志中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,BERR,8);         // [8]     总线错误中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,I2C,7);          // [7]     I2C中断标志/掩码位（原表12C应为I2C）
        DECLARE_BIT_FIELD(IFR1,TINT1,6);        // [6]     定时器1中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,DMAC3,5);        // [5]     DMA通道3中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,DMAC2,4);        // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
        DECLARE_BIT_FIELD(IFR1,INT4_RTC,3);     // [3]     外部中断4或实时时钟中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,DMAC0,2);        // [2]     DMA通道0中断标志/掩码位
        DECLARE_BIT_FIELD(IFR1,XINT0,1);        // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
        DECLARE_BIT_FIELD(IFR1,INT1,0);         // [0]     外部用户中断1标志/掩码位


        DECLARE_BITS_FIELD(IER1,RSVED, 5, 11);  // [15:11] 保留位（必须写0）
        DECLARE_BIT_FIELD(IER1,RTOS,10);        // [10]    实时操作系统中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,DLOG,9);         // [9]     数据日志中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,BERR,8);         // [8]     总线错误中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,I2C,7);          // [7]     I2C中断标志/掩码位（原表12C应为I2C）
        DECLARE_BIT_FIELD(IER1,TINT1,6);        // [6]     定时器1中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,DMAC3,5);        // [5]     DMA通道3中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,DMAC2,4);        // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
        DECLARE_BIT_FIELD(IER1,INT4_RTC,3);     // [3]     外部中断4或实时时钟中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,DMAC0,2);        // [2]     DMA通道0中断标志/掩码位
        DECLARE_BIT_FIELD(IER1,XINT0,1);        // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
        DECLARE_BIT_FIELD(IER1,INT1,0);         // [0]     外部用户中断1标志/掩码位
    }

    namespace isr
    {
      INLINE void start_timer()
      {

          //cpu::IVPD::write(0x01);
          //cpu::IVPH::write(0x01);
          cpu::IER0::TINT0::set_bit();
          cpu::DBIER0::write(cpu::DBIER0::read()|0x10);
          cpu::IFR0::write(0xFFFF);
          asm(" BCLR INTM");

      }



    }



}

#endif //ZQ_INTERRUPT_H
