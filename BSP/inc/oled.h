//
// Created by fairy on 2025/2/28 00:13.
//
#ifndef DEMO_LCD_H
#define DEMO_LCD_H
#include<zq_conf.h>
#include<zq_delay.h>
#include<zq_systick.h>

// 原本278ms 换成C++后需要340ms
// 延时
// 100us：   428ms
// 10us:     46.8ms
// 1us:      8.6ms      [异常]
// 5us:      26.4ms
// 2us:      13.2ms     [些许异常]
// 3us:      17.4ms     [最佳]

namespace bsp
{
    namespace oled
    {
        namespace detail
        {
            DECLARE_EXMEM_REGISTER_T(CTRL, 0x600801); // 如果要写入数据或者命令，那么该寄存器需要置零
            DECLARE_EXMEM_REGISTER_T(CMD, 0x602800); // 命令寄存器
            DECLARE_EXMEM_REGISTER_T(DATA_BASE, 0x602801);

            DECLARE_EXMEM_REGISTER_T(DATA_L, 0x602801); // 左侧的显示屏
            DECLARE_EXMEM_REGISTER_T(DATA_R, 0x600802); // 右侧的显示屏

            DECLARE_EXMEM_REGISTER_T(CTR_GR, 0x600800);

            DECLARE_EXMEM_REGISTER_T(CTR_KEY, 0x602800);

            DECLARE_EXMEM_REGISTER_T(CTR_LR, 0x602803);

            DECLARE_ATTRIBUTE(OP_CMD,
                              ON = 0x3f,
                              OFF = 0x3e,
                              START_LINE = 0xc0, // 设置显示起始行 0xc0 + (0~63)
                              PAGE = 0xb8, // 设置操作页 0xb8 + (0~7)
                              VER_ADDRESS = 0x40 // 设置操作列 0x40 + (0~63)
            );
        }
    }



    class OLED
    {
        // 寄存器
    public:
        INLINE void init()
        {
            oled::detail::CTR_GR::write(0);
            oled::detail::CTR_GR::write(0x80);
            oled::detail::CTR_GR::write(0);
            oled::detail::CTR_LR::write(0);
            oled::detail::CTR_LR::write(0x40);
            oled::detail::CTR_LR::write(0xc0);
            write_cmd(oled::detail::OP_CMD::OFF);
            zq::systick::Delay::ms(1);
            write_cmd(oled::detail::OP_CMD::ON);
        }

        /**
         * 向左侧屏幕写入数据
         * @param data 数据
         */
        INLINE void write_data_left(const uint16_t data)
        {
            oled::detail::DATA_L::write(data);
            zq::systick::Delay::us(3);
            oled::detail::CTRL::write(0);
            zq::systick::Delay::us(3);
        }

        /**
         * 向右侧屏幕写入数据
         * @param data 数据
         */
        INLINE void write_data_right(const uint16_t data)
        {
            oled::detail::DATA_R::write(data);
            zq::systick::Delay::us(3);
            oled::detail::CTRL::write(0);
            zq::systick::Delay::us(3);
        }

        /**
         * 写入命令
         * @param cmd 命令
         */
        INLINE void write_cmd(const uint16_t cmd)
        {
            oled::detail::CMD::write(cmd);
            zq::systick::Delay::us(3);
            oled::detail::CTRL::write(0);
            zq::systick::Delay::us(3);
        }

        /**
        * 设置当前行(页0~7)
        */
        INLINE void set_page(const uint16_t page)
        {
            write_cmd(oled::detail::OP_CMD::PAGE + (page & 0x07)); // 限制 page 在 0~7
        }

        // 设置当前列 (0~63)
        INLINE void set_column(const uint16_t column)
        {
            write_cmd(oled::detail::OP_CMD::VER_ADDRESS + column); // 左半屏
        }

        INLINE void clear()
        {
            write_cmd(oled::detail::OP_CMD::START_LINE);
            for (uint16_t i = 0; i < 8; ++i)
            {
                set_page(i);
                set_column(0);
                for (int j = 0; j < 64; ++j)
                {
                    write_data_left(0);
                    write_data_right(0);
                }
            }
        }

        INLINE void write_data(const unsigned char page, const unsigned char *buf)
        {
            write_cmd(oled::detail::OP_CMD::START_LINE);
            set_page(page);
            set_column(0);
            for (uint16_t j = 0; j < 64; ++j)
            {
                write_data_left(buf[j]);
                write_data_right(buf[j + 64]);
            }
        }
    };
}
#endif //DEMO_LCD_H
