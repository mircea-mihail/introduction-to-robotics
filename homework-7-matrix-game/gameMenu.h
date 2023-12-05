#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "utility.h"

class gameMenu
{
private:
    int m_state;
    // const int RESET = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
    LiquidCrystal m_lcd = LiquidCrystal(RESET, ENABLE, DATA4, DATA5, DATA6, DATA7);

public:
    gameMenu();
};

#endif