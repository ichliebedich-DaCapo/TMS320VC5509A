//
// Created by fairy on 2025/4/26 22:47.
//
#ifndef KEY_HPP
#define KEY_HPP
#include <zq_interrupt.h>

#include "oled.h"


namespace bsp
{
    namespace key
    {
        namespace detail
        {
            DECLARE_EXEMEMORY(KEY,0x602800);
            DECLARE_EXEMEMORY(KEY_CTRL,0x602801);
        }
    }
    class Key
    {
        static  uint16_t sign;
        static uint16_t code;
    public:

        static void init()
        {
            zq::mmr::IER0::INT2::set();
            zq::mmr::DBIER0::INT2::set();

            setCode(key::detail::KEY_CTRL::read());// 读键寄存器清零
            oled::detail::CTR_LR::write(0xC8);// 使能控制板
            setCode(key::detail::KEY_CTRL::read());// 读键寄存器清零
        }

        static uint16_t getCode(){return code;}

        static void setCode(const uint16_t code_){code = code_;}

        static void setSign(){sign = 1;}

        // 读取并清除标志位
        static bool readSign()
        {
            if (sign)
            {
                sign = 0;
                return true;
            }
            return false;
        }
    };
}



#endif //KEY_HPP