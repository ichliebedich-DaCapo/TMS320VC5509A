//
// Created by fairy on 2025/3/30 18:25.
//
#ifndef ZQ_CPU_H
#define ZQ_CPU_H
#include<zq_conf.h>
namespace zq
{
    namespace mmr
    {
        // C55x的一堆CPU寄存器映射
        /******************** 中断控制类寄存器 ​********************/
        DECLARE_EXEMEMORY(IER0,0x0000);   // 中断使能寄存器0   [15-0]
        DECLARE_EXEMEMORY(IFR0,0x0001);   // 中断标志寄存器0   [15-0]

        /******************** 状态寄存器组 ​***********************/
        DECLARE_EXEMEMORY(ST0_55,0x0002);  // C55x状态寄存器0  [15-0]
        DECLARE_EXEMEMORY(ST1_55,0x0003);  // C55x状态寄存器1  [15-0]
        DECLARE_EXEMEMORY(ST3_55,0x0004);  // C55x状态寄存器3  [15-0]
        DECLARE_EXEMEMORY(RSVD05,0x0005); // 保留区域          [15-0]
        DECLARE_EXEMEMORY(ST0,0x0006);     // 标准状态寄存器0  [15-0]
        DECLARE_EXEMEMORY(ST1,0x0007);     // 标准状态寄存器1  [15-0]

        /******************** 累加器寄存器组 ​*********************/
        //-- 累加器0分解寄存器 --
        DECLARE_EXEMEMORY(AC0L,0x0008);   // 低16位数据      [15-0]
        DECLARE_EXEMEMORY(AC0H,0x0009);   // 高16位数据      [31-16]
        DECLARE_EXEMEMORY(AC0G,0x000A);   // 保护位          [39-32]

        //-- 累加器1分解寄存器 --
        DECLARE_EXEMEMORY(AC1L,0x000B);   // 低16位数据      [15-0]
        DECLARE_EXEMEMORY(AC1H,0x000C);   // 高16位数据      [31-16]
        DECLARE_EXEMEMORY(AC1G,0x000D);   // 保护位          [39-32]

        /******************** 运算辅助寄存器 ​*********************/
        DECLARE_EXEMEMORY(TREG,0x000E);   // 临时数据寄存器    [15-0]（原T3）
        DECLARE_EXEMEMORY(TRN0,0x000F);   // 过渡寄存器        [15-0]

        /******************** 地址指针寄存器 ​*********************/
        DECLARE_EXEMEMORY(AR0,0x0010);    // 辅助寄存器0       [15-0]
        DECLARE_EXEMEMORY(AR1,0x0011);    // 辅助寄存器1       [15-0]
        DECLARE_EXEMEMORY(AR2,0x0012);    // 辅助寄存器2       [15-0]
        DECLARE_EXEMEMORY(AR3,0x0013);    // 辅助寄存器3       [15-0]
        DECLARE_EXEMEMORY(AR4,0x0014);    // 辅助寄存器4       [15-0]
        DECLARE_EXEMEMORY(AR5,0x0015);    // 辅助寄存器5       [15-0]
        DECLARE_EXEMEMORY(AR6,0x0016);    // 辅助寄存器6       [15-0]
        DECLARE_EXEMEMORY(AR7,0x0017);    // 辅助寄存器7       [15-0]


        // 系统控制类寄存器
        DECLARE_EXEMEMORY(SP,   0x0018);   // 栈指针寄存器           [15-0]
        DECLARE_EXEMEMORY(BK03,0x0019);   // 循环缓冲区大小寄存器    [15-0]
        DECLARE_EXEMEMORY(BRC0,0x001A);   // 块重复计数器          [15-0]
        DECLARE_EXEMEMORY(RSAOL,0x001B);  // 块重复起始地址寄存器   [15-0]
        DECLARE_EXEMEMORY(REAOL,0x001C);   // 块重复结束地址寄存器   [15-0]
        DECLARE_EXEMEMORY(PMST,0x001D);   // 处理器模式状态寄存器   [15-0]
        DECLARE_EXEMEMORY(XPC, 0x001E);    // 程序计数器扩展寄存器  [7-0]
        DECLARE_EXEMEMORY(RSVD1F,0x001F); // 保留寄存器           [15-0]

        // 数据操作类寄存器
        DECLARE_EXEMEMORY(T0,  0x0020);    // 临时数据寄存器0      [15-0]
        DECLARE_EXEMEMORY(T1,  0x0021);    // 临时数据寄存器1      [15-0]
        DECLARE_EXEMEMORY(T2,  0x0022);    // 临时数据寄存器2      [15-0]
        DECLARE_EXEMEMORY(T3,  0x0023);    // 临时数据寄存器3      [15-0]

        // 累加器扩展（AC2）
        DECLARE_EXEMEMORY(AC2L,0x0024);   // 累加器2低16位       [15-0]
        DECLARE_EXEMEMORY(AC2H,0x0025);   // 累加器2高16位       [31-16]
        DECLARE_EXEMEMORY(AC2G,0x0026);   // 累加器2保护位       [39-32]

        // 数据指针类寄存器
        DECLARE_EXEMEMORY(CDP, 0x0027);  // 系数数据指针           [15-0]

        // 累加器扩展（AC3）
        DECLARE_EXEMEMORY(AC3L, 0x0028);  // 累加器3低16位        [15-0]
        DECLARE_EXEMEMORY(AC3H, 0x0029);  // 累加器3高16位        [31-16]
        DECLARE_EXEMEMORY(AC3G, 0x002A);  // 累加器3保护位        [39-32]

        // 内存管理单元
        DECLARE_EXEMEMORY(DPH,  0x002B);  // 扩展数据页指针        [6-0]
        DECLARE_EXEMEMORY(MDP05,0x002C); // 保留寄存器           [6-0]（原MDP05）
        DECLARE_EXEMEMORY(MDP67,0x002D); // 保留寄存器           [15-0]（原MDP67）
        DECLARE_EXEMEMORY(DP,   0x002E);  // 二级数据页指针        [15-0]
        DECLARE_EXEMEMORY(PDP,  0x002F);  // 外设数据页起始地址     [8-0]

        // 循环缓冲控制类
        DECLARE_EXEMEMORY(BK47, 0x0030);  // AR4-7循环缓冲区大小    [15-0]
        DECLARE_EXEMEMORY(BKC,  0x0031);  // CDP循环缓冲区大小     [15-0]
        DECLARE_EXEMEMORY(BSA01,0x0032);  // AR0-1循环起始地址     [15-0]
        DECLARE_EXEMEMORY(BSA23,0x0033);  // AR2-3循环起始地址     [15-0]
        DECLARE_EXEMEMORY(BSA45,0x0034);  // AR4-5循环起始地址     [15-0]
        DECLARE_EXEMEMORY(BSA67,0x0035);  // AR6-7循环起始地址     [15-0]
        DECLARE_EXEMEMORY(BSAC, 0x0036);  // 系数循环起始地址      [15-0]
        DECLARE_EXEMEMORY(BIOS, 0x0037);  // 数据页指针存储（128字数据表）[15-0]

        // 特殊功能寄存器
        DECLARE_EXEMEMORY(TRN1, 0x0038);  // 过渡寄存器1          [15-0]（原BIOS）
        DECLARE_EXEMEMORY(BRC1,0x0039);  // 块重复计数器1        [15-0]
        DECLARE_EXEMEMORY(BRS1,0x003A);  // 块重复保存寄存器1     [15-0]
        DECLARE_EXEMEMORY(CSR, 0x003B);  // 单次重复计算寄存器    [15-0]
        DECLARE_EXEMEMORY(RSA0H,0x003C); // 块重复起始地址高位    [23-16]

        // 重复地址类寄存器
        DECLARE_EXEMEMORY(RSA0L,  0x003D);  // 重复起始地址0低16位   [15-0]
        DECLARE_EXEMEMORY(REA0H,  0x003E);  // 重复起始地址0高8位    [23-16]
        DECLARE_EXEMEMORY(REA0L,  0x003F);  // 重复结束地址0低16位   [15-0]
        DECLARE_EXEMEMORY(RSA1H,  0x0040);  // 重复起始地址1高8位    [23-16]
        DECLARE_EXEMEMORY(RSA1L,  0x0041);  // 重复起始地址1低16位   [15-0]
        DECLARE_EXEMEMORY(REA1H,  0x0042);  // 重复结束地址1高8位    [23-16]
        DECLARE_EXEMEMORY(REA1L,  0x0043);  // 重复结束地址1低16位   [15-0]

        // 控制计数器类
        DECLARE_EXEMEMORY(RPTC,   0x0044);  // 重复计数器            [15-0]

        // 中断调试类
        DECLARE_EXEMEMORY(IER1,   0x0045);  // 中断使能寄存器1       [15-0]
        DECLARE_EXEMEMORY(IFR1,   0x0046);  // 中断标志寄存器1       [15-0]
        DECLARE_EXEMEMORY(DBIER0, 0x0047);  // 调试中断使能寄存器0   [15-0]
        DECLARE_EXEMEMORY(DBIER1, 0x0048);  // 调试中断使能寄存器1   [15-0]

        // 系统类寄存器
        DECLARE_EXEMEMORY(IVPD,   0x0049);  // 中断向量页指针        [15-0]
        DECLARE_EXEMEMORY(IVPH,   0x004A);  // 中断向量页指针       [15-0]
        DECLARE_EXEMEMORY(ST2_55, 0x004B);  // 保留区域             [15-0]
        DECLARE_EXEMEMORY(SSP,    0x004C);  // 系统堆栈指针          [15-0]
        DECLARE_EXEMEMORY(USP,     0x004D);  // 用户堆栈指针          [15-0](原为SP)
        DECLARE_EXEMEMORY(SPH,    0x004E);  // 扩展堆栈页指针        [6-0]
        DECLARE_EXEMEMORY(CDPH,   0x004F);  // CDP高位页指针         [6-0]
    }
}

#endif //ZQ_CPU_H