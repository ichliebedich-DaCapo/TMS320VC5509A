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
        namespace IFR0
        {
            DECLARE_BITS_FIELD(DMAC5, 0x8000, 15);  // DMA通道5中断标志/屏蔽位
            DECLARE_BITS_FIELD(DMAC4, 0x4000, 14);  // DMA通道4中断标志/屏蔽位
            DECLARE_BITS_FIELD(XINT2, 0x2000, 13);  // XINT2外部中断或MMC/SD2中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT2, 0x1000, 12);  // McBSP2接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(INT3, 0x0800, 11);   // 外部中断3或看门狗定时器中断标志/屏蔽位
            DECLARE_BITS_FIELD(DSPINT, 0x0400, 10); // HPI主机到DSP的中断标志/屏蔽位
            DECLARE_BITS_FIELD(DMAC1, 0x0200, 9);   // DMA通道1中断标志/屏蔽位
            DECLARE_BITS_FIELD(USB, 0x0100, 8);     // USB中断标志/屏蔽位
            DECLARE_BITS_FIELD(XINT1, 0x0080, 7);   // XINT1外部中断或MMC/SD1中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT1, 0x0040, 6);   // McBSP1接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT0, 0x0020, 5);   // McBSP0接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(TINT0, 0x0010, 4);   // 定时器0中断标志位
            DECLARE_BITS_FIELD(INT2, 0x0008, 3);    // 外部中断2标志位
            DECLARE_BITS_FIELD(INT0, 0x0004, 2);    // 外部中断1标志位（注意表格中疑似标注错误）
            DECLARE_BITS_FIELD(RSVED, 0x0003, 0);   // 位1-0为保留位（0x0002-0x0001），需保持写0
        }

        // 中断使能寄存器
        namespace IER0
        {
            DECLARE_BITS_FIELD(DMAC5, 0x8000, 15);  // DMA通道5中断标志/屏蔽位
            DECLARE_BITS_FIELD(DMAC4, 0x4000, 14);  // DMA通道4中断标志/屏蔽位
            DECLARE_BITS_FIELD(XINT2, 0x2000, 13);  // XINT2外部中断或MMC/SD2中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT2, 0x1000, 12);  // McBSP2接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(INT3, 0x0800, 11);   // 外部中断3或看门狗定时器中断标志/屏蔽位
            DECLARE_BITS_FIELD(DSPINT, 0x0400, 10); // HPI主机到DSP的中断标志/屏蔽位
            DECLARE_BITS_FIELD(DMAC1, 0x0200, 9);   // DMA通道1中断标志/屏蔽位
            DECLARE_BITS_FIELD(USB, 0x0100, 8);     // USB中断标志/屏蔽位
            DECLARE_BITS_FIELD(XINT1, 0x0080, 7);   // XINT1外部中断或MMC/SD1中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT1, 0x0040, 6);   // McBSP1接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(RINT0, 0x0020, 5);   // McBSP0接收中断标志/屏蔽位
            DECLARE_BITS_FIELD(TINT0, 0x0010, 4);   // 定时器0中断标志位
            DECLARE_BITS_FIELD(INT2, 0x0008, 3);    // 外部中断2标志位
            DECLARE_BITS_FIELD(INT0, 0x0004, 2);    // 外部中断1标志位（注意表格中疑似标注错误）
            DECLARE_BITS_FIELD(RSVED, 0x0003, 0);   // 位1-0为保留位（0x0002-0x0001），需保持写0
        }

        namespace IFR1
        {
            DECLARE_BITS_FIELD(RSVED, 0xF800, 11);   // [15:11] 保留位（必须写0）
            DECLARE_BITS_FIELD(RTOS, 0x0400, 10);    // [10]    实时操作系统中断标志/掩码位
            DECLARE_BITS_FIELD(DLOG, 0x0200, 9);     // [9]     数据日志中断标志/掩码位
            DECLARE_BITS_FIELD(BERR, 0x0100, 8);     // [8]     总线错误中断标志/掩码位
            DECLARE_BITS_FIELD(I2C, 0x0080, 7);      // [7]     I2C中断标志/掩码位（原表12C应为I2C）
            DECLARE_BITS_FIELD(TINT1, 0x0040, 6);    // [6]     定时器1中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC3, 0x0020, 5);    // [5]     DMA通道3中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC2, 0x0010, 4);    // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
            DECLARE_BITS_FIELD(INT4_RTC, 0x0008, 3); // [3]     外部中断4或实时时钟中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC0, 0x0004, 2);    // [2]     DMA通道0中断标志/掩码位
            DECLARE_BITS_FIELD(XINT0, 0x0002, 1);    // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
            DECLARE_BITS_FIELD(INT1, 0x0001, 0);     // [0]     外部用户中断1标志/掩码位
        }

        namespace IER1
        {
            DECLARE_BITS_FIELD(RSVED, 0xF800, 11);   // [15:11] 保留位（必须写0）
            DECLARE_BITS_FIELD(RTOS, 0x0400, 10);    // [10]    实时操作系统中断标志/掩码位
            DECLARE_BITS_FIELD(DLOG, 0x0200, 9);     // [9]     数据日志中断标志/掩码位
            DECLARE_BITS_FIELD(BERR, 0x0100, 8);     // [8]     总线错误中断标志/掩码位
            DECLARE_BITS_FIELD(I2C, 0x0080, 7);      // [7]     I2C中断标志/掩码位（原表12C应为I2C）
            DECLARE_BITS_FIELD(TINT1, 0x0040, 6);    // [6]     定时器1中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC3, 0x0020, 5);    // [5]     DMA通道3中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC2, 0x0010, 4);    // [4]     DMA通道2中断标志/掩码位（原表RMAC2应为DMAC2）
            DECLARE_BITS_FIELD(INT4_RTC, 0x0008, 3); // [3]     外部中断4或实时时钟中断标志/掩码位
            DECLARE_BITS_FIELD(DMAC0, 0x0004, 2);    // [2]     DMA通道0中断标志/掩码位
            DECLARE_BITS_FIELD(XINT0, 0x0002, 1);    // [1]     McBSP发送0中断标志/掩码位（原表XINTO应为XINT0）
            DECLARE_BITS_FIELD(INT1, 0x0001, 0);     // [0]     外部用户中断1标志/掩码位
        }
    }

    namespace isr
    {
       DECLARE_REGISTER(IER0, 0x0);



    }

    namespace timer
    {
        // TIM0寄存器
        DECLARE_REGISTER(TIM0, 0x1000);

        // TCR0寄存器
        DECLARE_REGISTER(TCR0, 0x1002);

        // TCRO寄存器位域
        namespace TCR0
        {

        };

        class Timer0
        {
            // 寄存器
            typedef mmio::RegAccess<TCR0::REG> TCR0_REG;
        public:
            // 设置IDLE是否使能   0:禁止进入IDLE状态   1：允许进入IDLE状态
            template<uint16_t mode>
            static void set_idle_mode()
            {
                TCR0_REG::modify_bits(mode, TCR0::IDLEEN::MASK, TCR0::IDLEEN::SHIFT);
            }
        };

    }


}

#endif //ZQ_INTERRUPT_H
