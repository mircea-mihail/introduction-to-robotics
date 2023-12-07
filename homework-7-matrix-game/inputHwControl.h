#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>
#include "utility.h"

// button
#define DEBOUNCE_TIME 50

class inputHwControl
{
private:

    struct button
    {
        int m_prevState = LOW;
        int m_prevCountedState = LOW;
        unsigned long m_prevTime = 0;
    };

    button m_btn;

public:
    bool pressedButton();

    bool joystickDetected(int &p_xCommand, int &p_yCommand);
    
    int getBrightness();
};

#endif