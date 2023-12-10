#include "gameMenu.h"

int gameMenu::menuSequence()
{
    // only do intro/outro message seq in menu sequence;
    if(doIntroMessageSequence())
    {
        return m_state;
    }
    if(doEndMessageSequence())
    {
        return m_state;
    }

    if(m_state != MENU_IN_GAME && !m_inSubmenu)
    {
        goToNextMenuOption(m_state, MENU_IN_START_GAME, MENU_IN_ABOUT);
    }

    switch (m_state)
    {
    case MENU_IN_START_GAME:
        if(m_changedState)
        {
            g_map.displayIcon(ICON_HAMMER);
            
            m_lcd.print(F("   start game"));
            m_changedState = false;
        }

        if(m_hwCtrl.pressedButton())
        {
            changeState(m_state, MENU_IN_GAME);   
        }
        break;

    case MENU_IN_SETTINGS:
        if(m_settingsState == RETURN_FROM_SETTINGS)
        {
            if(m_changedState)
            {
                g_map.displayIcon(ICON_HAMMER);

                m_lcd.print(F("    settings   >"));
                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
                m_lcd.print(F("               >"));

                m_changedState = false;
                m_inSubmenu = false;
            }

            if(m_hwCtrl.joystickRight() && m_hwCtrl.pressedButton())
            {
                m_inSubmenu = true;
                changeState(m_settingsState, IN_MATRIX_BRIGHTNESS);
            }
        }
        else
        {
            goToSettingsMenu();
        }
        break;

    case MENU_IN_ABOUT:
        if(m_changedState)
        {
            g_map.displayIcon(ICON_HAMMER);

            if(!m_showAboutText)
            {
                m_lcd.print(F("     about"));

                m_changedState = false;
            }
            else
            {
                m_lcd.print(F("Rocket Cowboy by Mircea"));
                m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_COL);
                m_lcd.print(F("github.com/mircea-mihail for more info"));
                m_changedState = false;
            }
        }
        if(m_showAboutText)
        {
            if(m_hwCtrl.joystickLeft())
            {
                if(millis() - m_lcdScrollChange > CYCLE_DELAY_MILLIS)
                {
                    m_lcd.scrollDisplayRight();
                    m_lcdScrollChange = millis();
                }
            }
            if(m_hwCtrl.joystickRight())
            {
                if(millis() - m_lcdScrollChange > CYCLE_DELAY_MILLIS)
                {
                    m_lcd.scrollDisplayLeft();
                    m_lcdScrollChange = millis();
                }
            }
        }

        if(m_hwCtrl.pressedButton())
        {
            m_showAboutText = !m_showAboutText;
            m_lcd.clear();
            m_changedState = true;
        }
        break; 

    case MENU_IN_GAME:
        if(m_changedState)
        {
            m_lcd.print(F("shoot them ALL!!"));
            m_changedState = false;
        }
        break;    
    
    default:
        break;
    }

    return m_state;
}  

void gameMenu::goToSettingsMenu()
{
    goToNextMenuOption(m_settingsState, IN_MATRIX_BRIGHTNESS, IN_LCD_BRIGHTNESS);

    if(m_hwCtrl.joystickLeft() && m_hwCtrl.pressedButton())
    {
        m_inSubmenu = false;
        changeState(m_settingsState, RETURN_FROM_SETTINGS);
    }

    switch (m_settingsState)
    {
    case IN_MATRIX_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("<  matrix sun"));
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("< "));
            printHashesForMatrix(g_map.getMatrixBrightness());

            m_changedState = false;
        }
        
        if(m_hwCtrl.joystickLeft())
        {
            if(millis() - m_lastMatrixBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastMatrixBrightnessChange = millis();
                byte matrixBrightness = g_map.decrementMatrixBrightness();
                printHashesForMatrix(matrixBrightness);
                Serial.print(matrixBrightness);
            }
        }

        if(m_hwCtrl.joystickRight())
        {
            if(millis() - m_lastMatrixBrightnessChange > CYCLE_DELAY_MILLIS)
            {
                m_lastMatrixBrightnessChange = millis();
                byte matrixBrightness = g_map.incrementMatrixBrightness();
                printHashesForMatrix(matrixBrightness);
                Serial.print(matrixBrightness);
            }
        }

        break;

    case IN_LCD_BRIGHTNESS:
        if(m_changedState)
        {
            m_lcd.print(F("<   lcd  sun"));
            m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);
            m_lcd.print(F("< "));
            printHashesLCD(m_lcdContrast);
            m_changedState = false;
        }

        if(m_hwCtrl.joystickLeft())
        {
            if(millis() - m_lastContrastChange > CYCLE_DELAY_MILLIS)
            {
                m_lastContrastChange = millis();
                m_lcdContrast += CONTRAST_INCREMENT_VAL;
                printHashesLCD(m_lcdContrast);

                analogWrite(LCD_CONTRAST, m_lcdContrast);
            }
        }

        if(m_hwCtrl.joystickRight())
        {
            if(millis() - m_lastContrastChange > CYCLE_DELAY_MILLIS)
            {
                m_lastContrastChange = millis();
                m_lcdContrast -= CONTRAST_INCREMENT_VAL;
                printHashesLCD(m_lcdContrast);

                analogWrite(LCD_CONTRAST, m_lcdContrast);
            }
        }
        break;

    case RETURN_FROM_SETTINGS:
        break;
    
    default:
        break;
    }
}