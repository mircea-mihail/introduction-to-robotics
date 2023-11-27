#include "peripherals.h"

peripherals::peripherals()
{
    pinMode(BRIGHTNESS_PIN, INPUT);
    pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);
}

int peripherals::getBrightness()
{
    return analogRead(BRIGHTNESS_PIN);
}