//
// Created by fairy on 2025/3/3 01:09.
//

#ifndef SIMULATOR
#include <lcd.h>
#endif
#include "gui.h"



void GUI_Init()
{
#ifndef SIMULATOR
    lcd_init();
#endif

}


