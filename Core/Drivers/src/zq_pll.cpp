//
// Created by fairy on 2025/3/2 01:11.
//
#include "zq_pll.h"



/**
 * @brief 设置 USB PLL 锁相环时钟频率
 * @note 后续如果想要扩展也很简单，因为这个锁相环很简单。那么只需要把所有能设置的时钟频率系数枚举出来即可，比如1/2、5/2等。
 *      总共就这两种可能性：（1~15）/2     1~15
 * APLL配置逻辑遵循以下表格规则：
 * ------------------------------------------------------------
 * | MODE | DIV | K       | M    | D    | 说明                |
 * |------|-----|---------|------|------|---------------------|
 * | 0    | X   | 1-15    | 1    | 2    | 强制分频 (M/D=1/2)  |
 * | 0    | X   | 16      | 1    | 4    | 强制分频 (M/D=1/4)  |
 * | 1    | 0   | 1-15    | K    | 1    | 直接倍频 (M/D=K/1)  |
 * | 1    | 0   | 16      | 1    | 1    | 特殊处理 (M/D=1/1)  |
 * | 1    | 1   | 奇数     | K    | 2    | 奇数倍频 (M/D=K/2) |
 * | 1    | 1   | 偶数     | K-1  | 4    | 抑制噪声 (M/D=(K-1)/4) |
 * ------------------------------------------------------------
 *
 * @note 最终频率公式: F_USB_APLLCLK = F_CLKIN * (M/D)
 */
void ZQ_USBPLL_Init()
{
    // 选择USB APLL
    ZQ_USBPLL_Select(USBPLL_APLL);

    // 设置为倍频模式
    ZQ_USBPLL_APLL_Set_Mode(USBPLL_APLL_MODE_MULT);

    // 设置1分频
    ZQ_USBPLL_APLL_Set_DIV(USBPLL_APLL_DIV_1);

    // 设置倍频系数为10
    ZQ_USBPLL_APLL_Set_MULT(USBPLL_APLL_MULT_10);

    // 启用APLL
    ZQ_USBPLL_APLL_ENABLE();

    // 等待APLL稳定
    while (!ZQ_USBPLL_APLL_Get_Stat());
}

/**
 * @brief 设置数字 PLL 锁相环时钟频率
 * @note 为了简化配置，默认启用APLL，并且已经配置为最高可用频率。此处CVDD接1.6V，因此最高推荐频率为200MHz，
 *       而外部晶振为20MHz，因此倍频为10
 */
void ZQ_PLL_Init()
{
    ZQ_PLL_DISABLE(); // 关闭PLL
    while (ZQ_PLL_Get_LOCK_FLAG()); // 等待退出PLL模式

    ZQ_PLL_Set_IAI(PLL_IAI_SAME); // 设置PLL退出Idle后，使用上次设置，快速锁相
    ZQ_PLL_Set_IOB(PLL_IOB_SWITCH_BYPASS); // 设置PLL应急模式，如果发生失锁，就切换为旁路模式
    ZQ_PLL_Set_TEST(); // TEST位必须置为0
    ZQ_PLL_Set_DIV(PLL_DIV_1); // 不分频
    ZQ_PLL_Set_MULT(PLL_MULT_10); // 十倍频
    ZQ_PLL_Set_BYPASS_DIV(PLL_BYPASS_DIV_1); // 旁路模式下不分频

    while (!ZQ_PLL_Get_LOCK_FLAG()); // 等待PLL锁定

    ZQ_PLL_Set_CLKOUT_DIV(PLL_CLKOUT_DIV_4);// 设置输出时钟分频为4
}



