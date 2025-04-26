//
// Created by fairy on 2025/4/26 00:43.
//
#ifndef ZQ_I2C_H
#define ZQ_I2C_H
#include <zq_conf.h>


namespace zq
{
    namespace i2c
    {
        namespace detail
        {
            // I2C寄存器声明
            DECLARE_REGISTER(OAR,  0x3C00);  // [9:0] I²C自有地址寄存器
            DECLARE_REGISTER(IER,  0x3C01);  // I²C中断使能寄存器
            DECLARE_REGISTER(STR,  0x3C02);  // I²C状态寄存器（复位值含特殊位）
            DECLARE_REGISTER(CLKL,  0x3C03);  // I²C分频寄存器低位
            DECLARE_REGISTER(CLKH, 0x3C04);  // I²C分频寄存器高位
            DECLARE_REGISTER(CNT,  0x3C05);  // I²C数据计数寄存器
            DECLARE_REGISTER(DR,   0x3C06);  // I²C数据接收寄存器[7:0] 原名为DRR
            DECLARE_REGISTER(SA,  0x3C07);  // [9:0] I²C从机地址寄存器（复位值0x03FF） 原名为SAR
            DECLARE_REGISTER(DX,   0x3C08);  // I²C数据发送寄存器[7:0] 原名为DXR
            DECLARE_REGISTER(MD,  0x3C09);  // I²C模式寄存器[14:0] 原名为MDR
            DECLARE_REGISTER(ISRC, 0x3C0A);  // I²C中断源寄存器
            DECLARE_REGISTER(PSC,  0x3C0C);  // I²C预分频寄存器
            DECLARE_REGISTER(MDR2, 0x3C0F);  // I²C扩展模式寄存器

            // I2C Mode Register (I2CMDR) 位域声明
            DECLARE_REG_BIT(MD, FREE,   15);    // [15] 自由运行模式 (0=禁用,1=自动重复发送)
            DECLARE_REG_BIT(MD, STT,    14);    // [14] START条件生成 (0=无操作,1=产生START)
            DECLARE_REG_BIT(MD, IDLEEN, 13);    // [13] 空闲模式使能 (0=禁用,1=空闲时暂停)
            DECLARE_REG_BIT(MD, STP,    12);    // [12] STOP条件生成 (0=无操作,1=产生STOP)
            DECLARE_REG_BIT(MD, MST,    11);    // [11] 主从模式选择 (0=从机,1=主机)
            DECLARE_REG_BIT(MD, TRX,    10);    // [10] 传输方向 (0=接收模式,1=发送模式)
            DECLARE_REG_BIT(MD, XA,     9);     // [9]  地址扩展 (0=7位地址,1=10位地址)
            DECLARE_REG_BIT(MD, RM,     8);     // [8]  重复模式 (0=禁用,1=自动ACK生成)
            DECLARE_REG_BIT(MD, DLB,    7);     // [7]  数字环回 (0=正常模式,1=内部环回)
            DECLARE_REG_BIT(MD, IRS,    6);     // [6]  模块复位 (0=复位,1=使能模块)
            DECLARE_REG_BIT(MD, STB,    5);     // [5]  起始字节模式 (0=禁用,1=起始字节传输)
            DECLARE_REG_BIT(MD, FDF,    4);     // [4]  全双工模式 (0=半双工,1=全双工)
            DECLARE_REG_BIT(MD, BC,     0);     // [3:0] 字节计数器 (4位,设置传输字节数)


            // 状态寄存器 (I2CSTR) 位域声明
            DECLARE_REG_BIT(STR, BB,      0);  // [0]  总线忙标志 (只读) 0=空闲,1=忙
            DECLARE_REG_BIT(STR, RSFULL,  1);  // [1]  接收移位寄存器满 (只读) 0=未满,1=满
            DECLARE_REG_BIT(STR, XSMT,    2);  // [2]  发送移位寄存器空 (只读) 0=未空,1=空
            DECLARE_REG_BIT(STR, AAS,     3);  // [3]  从地址识别 (只读) 0=未识别,1=地址匹配
            DECLARE_REG_BIT(STR, AD0,     4);  // [4]  地址0状态 (只读) 0=不匹配,1=匹配
            DECLARE_REG_BIT(STR, XRDY,    5);  // [5]  发送就绪 (只读) 0=未就绪,1=可写DXR
            DECLARE_REG_BIT(STR, RRDY,    6);  // [6]  接收就绪 (只读) 0=无数据,1=可读DRR
            DECLARE_REG_BIT(STR, ARDY,    7);  // [7]  寄存器访问就绪 (只读) 0=忙,1=配置完成
            DECLARE_REG_BIT(STR, NACK,    8);  // [8]  无应答标志 (写清零) 0=正常,1=检测NACK
            DECLARE_REG_BIT(STR, AL,      9);  // [9]  仲裁丢失标志 (写清零) 0=正常,1=丢失仲裁
            DECLARE_REG_BIT(STR, GC,     10);  // [10] 广播呼叫识别 (只读) 0=未识别,1=识别到广播
            DECLARE_REG_FIELD(STR, Reserved,5, 11); // [15-11] 保留

        }

        class I2C {
        public:
            // 初始化配置结构体
            struct Config {
                uint32_t system_clock;    // 系统时钟频率(Hz)
                uint32_t bitrate;         // 目标比特率
                uint16_t own_address;      // 自身地址(从机模式使用)
                bool loopback;            // 环回模式使能
            };

            static void init(const Config& cfg) {
                using namespace detail;

                // 1. 复位并禁用模块
                MD::IRS::clear();         // 复位I2C模块
                while(MD::IRS::read_bit()) {}// 等待复位完成

                // 2. 配置时钟 (文档8中的分频计算)
                const uint16_t prescaler = cfg.system_clock / (12 * cfg.bitrate) - 1;
                PSC::write(prescaler);

                // 3. 设置SCL时序（文档3中的默认值）
                CLKL::write(15);  // 低电平周期
                CLKH::write(15);  // 高电平周期

                // 4. 配置模式寄存器（基于文档3的RMK宏）
                MD::FREE::write_bit(cfg.loopback);   // 自由运行模式
                MD::MST::set();         // 主模式
                MD::TRX::set();         // 默认发送模式
                MD::XA::clear();        // 7位地址模式
                MD::DLB::write_bit(cfg.loopback); // 数字环回

                // 5. 设置自身地址（从机模式备用）
                OAR::write(cfg.own_address);

                // 6. 使能模块
                MD::IRS::set();         // 使能I2C操作
            }

            // 发送启动信号（文档5中的STT操作）
            static void start() {
                using namespace detail;
                MD::STT::set();
                while(MD::STT::read_bit()); // 等待START完成
            }

            // 发送数据（文档3中的DXR操作）
            static void send(const uint16_t data) {
                using namespace detail;
                while(!STR::XRDY::read_bit()) {}
                // 等待发送就绪
                DX::write(data);
            }
        };
    }

}
#endif //ZQ_I2C_H
