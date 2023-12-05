#include "inputHwControl.h"

bool inputHwControl::pressedButton()
{
    unsigned long time = millis();
    int state = !digitalRead(BUTTON_PIN);

    // if the button has a constant state
    if(state == m_btn.m_prevState)
    {
        // if the constant state has been kept for a while
        if(time - m_btn.m_prevTime > DEBOUNCE_TIME && m_btn.m_prevCountedState != state)
        {
            m_btn.m_prevCountedState = state;

            if(state == HIGH)
            {
                m_btn.m_prevState = state;
                return true;
            }
        }
    }
    else
    {
        m_btn.m_prevTime = time;
    }

    m_btn.m_prevState = state;
    
    return false;
}

int inputHwControl::getBrightness()
{
    return analogRead(BRIGHTNESS_PIN);
}