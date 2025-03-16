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
// void ZQ_USBPLL_Init()
// {
//     // 选择USB APLL
//     ZQ_USBPLL_Select(USBPLL_APLL);
//
//     // 设置为倍频模式
//     ZQ_USBPLL_APLL_Set_Mode(USBPLL_APLL_MODE_MULT);
//
//     // 设置1分频
//     ZQ_USBPLL_APLL_Set_DIV(USBPLL_APLL_DIV_1);
//
//     // 设置倍频系数为10
//     ZQ_USBPLL_APLL_Set_MULT(USBPLL_APLL_MULT_10);
//
//     // 启用APLL
//     ZQ_USBPLL_APLL_ENABLE();
//
//     // 等待APLL稳定
//     while (!ZQ_USBPLL_APLL_Get_Stat());
// }



