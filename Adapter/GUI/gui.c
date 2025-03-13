//
// Created by fairy on 2025/3/3 01:09.
//
#include <stdio.h>
#ifdef SIMULATOR
#include<simulator.hpp>
#endif
#include "gui.h"
#include <lcd.h>


void GUI_Init()
{
#ifndef SIMULATOR
    lcd_init();
#endif

}


