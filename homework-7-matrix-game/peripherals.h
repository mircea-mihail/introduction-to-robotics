#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <Arduino.h>

#define BRIGHTNESS_PIN A2
#define PAUSE_BUTTON_PIN 4

// deals with all of the outside sensors and equipment not critical/ profoundly tied to the game
class peripherals
{

public:
    // constructor that initialises the peripherals pins
    peripherals();

    // returns the brightness detected by the brightness sensor
    int getBrightness();
};

#endif