//
// Created by fairy on 2025/3/30 18:12.
//
#ifndef ZQ_INTERRUPT_H
#define ZQ_INTERRUPT_H
#include<zq_conf.h>
#include<zq_mmr.h>

namespace zq
{
    // 补充相关寄存器的位域
    namespace mmr
    {
        // 中断标志寄存器
        DECLARE_EXEMEM_BIT(IFR0, DMAC5, 15);    // DMA通道5中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, DMAC4, 14);    // DMA通道4中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, XINT2, 13);    // XINT2外部中断或MMC/SD2中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, RINT2, 12);    // McBSP2接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, INT3, 11);     // 外部中断3或看门狗定时器中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, DSPINT, 10);   // HPI主机到DSP的中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, DMAC1, 9);     // DMA通道1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, USB, 8);       // USB中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, XINT1, 7);     // XINT1外部中断或MMC/SD1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, RINT1, 6);     // McBSP1接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, RINT0, 5);     // McBSP0接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IFR0, TINT0, 4);     // 定时器0中断标志位
        DECLARE_EXEMEM_BIT(IFR0, INT2, 3);      // 外部中断2标志位
        DECLARE_EXEMEM_BIT(IFR0, INT0, 2);      // 外部中断1标志位（注意表格中疑似标注错误）
        DECLARE_EXEMEM_FIELD(IFR0, RSVED, 2, 0);// 位1-0为保留位（0x0002-0x0001），需保持写0

        // 中断使能寄存器
        DECLARE_EXEMEM_BIT(IER0, DMAC5, 15);    // DMA通道5中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, DMAC4, 14);    // DMA通道4中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, XINT2, 13);    // XINT2外部中断或MMC/SD2中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, RINT2, 12);    // McBSP2接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, INT3, 11);     // 外部中断3或看门狗定时器中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, DSPINT, 10);   // HPI主机到DSP的中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, DMAC1, 9);     // DMA通道1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, USB, 8);       // USB中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, XINT1, 7);     // XINT1外部中断或MMC/SD1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, RINT1, 6);     // McBSP1接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, RINT0, 5);     // McBSP0接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(IER0, TINT0, 4);     // 定时器0中断标志位
        DECLARE_EXEMEM_BIT(IER0, INT2, 3);      // 外部中断2标志位
        DECLARE_EXEMEM_BIT(IER0, INT0, 2);      // 外部中断1标志位（注意表格中疑似标注错误）
        DECLARE_EXEMEM_BIT(IER0, RSVED, 0);     // 位1-0为保留位（0x0002-0x0001），需保持写0

        DECLARE_EXEMEM_BIT(DBIER0, DMAC5, 15);  // DMA通道5中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, DMAC4, 14);  // DMA通道4中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, XINT2, 13);  // XINT2外部中断或MMC/SD2中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, RINT2, 12);  // McBSP2接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, INT3, 11);   // 外部中断3或看门狗定时器中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, DSPINT, 10); // HPI主机到DSP的中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, DMAC1, 9);   // DMA通道1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, USB, 8);     // USB中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, XINT1, 7);   // XINT1外部中断或MMC/SD1中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, RINT1, 6);   // McBSP1接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, RINT0, 5);   // McBSP0接收中断标志/屏蔽位
        DECLARE_EXEMEM_BIT(DBIER0, TINT0, 4);   // 定时器0中断标志位
        DECLARE_EXEMEM_BIT(DBIER0, INT2, 3);    // 外部中断2标志位
        DECLARE_EXEMEM_BIT(DBIER0, INT0, 2);    // 外部中断1标志位（注意表格中疑似标注错误）
        DECLARE_EXEMEM_BIT(DBIER0, RSVED, 0);   // 位1-0为保留位（0x0002-0x0001），需保持写0

        DECLARE_EXEMEM_FIELD(IFR1, RSVED, 5, 11);   // [15:11] 保留位（必须写0）
        DECLARE_EXEMEM_BIT(IFR1, RTOS, 10);         // [10]    实时操作系统中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, DLOG, 9);          // [9]     数据日志中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, BERR, 8);          // [8]     总线错误中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, I2C, 7);           // [7]     I2C中断标志/掩码位（原表12C应为I2C）
        DECLARE_EXEMEM_BIT(IFR1, TINT1, 6);         // [6]     定时器1中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, DMAC3, 5);         // [5]     DMA通道3中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, DMAC2, 4);         // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
        DECLARE_EXEMEM_BIT(IFR1, INT4_RTC, 3);      // [3]     外部中断4或实时时钟中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, DMAC0, 2);         // [2]     DMA通道0中断标志/掩码位
        DECLARE_EXEMEM_BIT(IFR1, XINT0, 1);         // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
        DECLARE_EXEMEM_BIT(IFR1, INT1, 0);          // [0]     外部用户中断1标志/掩码位

        DECLARE_EXEMEM_FIELD(IER1, RSVED, 5, 11);   // [15:11] 保留位（必须写0）
        DECLARE_EXEMEM_BIT(IER1, RTOS, 10);         // [10]    实时操作系统中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, DLOG, 9);          // [9]     数据日志中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, BERR, 8);          // [8]     总线错误中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, I2C, 7);           // [7]     I2C中断标志/掩码位（原表12C应为I2C）
        DECLARE_EXEMEM_BIT(IER1, TINT1, 6);         // [6]     定时器1中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, DMAC3, 5);         // [5]     DMA通道3中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, DMAC2, 4);         // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
        DECLARE_EXEMEM_BIT(IER1, INT4_RTC, 3);      // [3]     外部中断4或实时时钟中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, DMAC0, 2);         // [2]     DMA通道0中断标志/掩码位
        DECLARE_EXEMEM_BIT(IER1, XINT0, 1);         // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
        DECLARE_EXEMEM_BIT(IER1, INT1, 0);          // [0]     外部用户中断1标志/掩码位

        DECLARE_EXEMEM_FIELD(DBIER1, RSVED, 5, 11); // [15:11] 保留位（必须写0）
        DECLARE_EXEMEM_BIT(DBIER1, RTOS, 10);       // [10]    实时操作系统中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, DLOG, 9);        // [9]     数据日志中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, BERR, 8);        // [8]     总线错误中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, I2C, 7);         // [7]     I2C中断标志/掩码位（原表12C应为I2C）
        DECLARE_EXEMEM_BIT(DBIER1, TINT1, 6);       // [6]     定时器1中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, DMAC3, 5);       // [5]     DMA通道3中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, DMAC2, 4);       // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
        DECLARE_EXEMEM_BIT(DBIER1, INT4_RTC, 3);    // [3]     外部中断4或实时时钟中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, DMAC0, 2);       // [2]     DMA通道0中断标志/掩码位
        DECLARE_EXEMEM_BIT(DBIER1, XINT0, 1);       // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
        DECLARE_EXEMEM_BIT(DBIER1, INT1, 0);        // [0]     外部用户中断1标志/掩码位
    }

    namespace isr
    {
        // 中断初始化
        INLINE void init()
        {
#if !defined(__ARM__)
            asm(" BSET INTM"); // BSET也就是置1，即禁止所有可屏蔽中断
#endif
            mmr::IVPD::write(0x02);
            mmr::IVPH::write(0x02);
#if !defined(__ARM__)
            asm(" BCLR INTM"); // 允许所有可屏蔽中断
#endif

            mmr::IFR0::write(0xffff); // 清除所有中断标志位
        }


    }
}

#endif //ZQ_INTERRUPT_H
