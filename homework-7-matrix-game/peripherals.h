#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <Arduino.h>

#define BRIGHTNESS_PIN A2
#define PAUSE_BUTTON_PIN 4

class peripherals
{

public:
    peripherals()
    {
        pinMode(BRIGHTNESS_PIN, INPUT);
        pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);

    }

    int getBrightness()
    {
        return analogRead(BRIGHTNESS_PIN);
    }
};

#endif