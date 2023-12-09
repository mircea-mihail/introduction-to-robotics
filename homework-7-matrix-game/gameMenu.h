#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "utility.h"
#include "inputHwControl.h"
#include "gameMap.h"

// lcd default characteristics
#define LCD_COLS 16
#define LCD_ROWS 2

#define CONTRAST_INCREMENT_VAL 16

// printing defines
#define FIRST_LCD_ROW 0
#define SECOND_LCD_ROW 1

#define FIRST_LCD_COL 0
#define SECOND_LCD_COL 1

// menu cycling
#define CYCLE_DELAY_MILLIS 400
#define INTRO_MESSAGE_MILLIS 3000
#define END_MESSAGE_MILLIS 3000

// settings submenu states:
#define IN_MATRIX_BRIGHTNESS 0
#define IN_LCD_BRIGHTNESS 1
#define RETURN_FROM_SETTINGS 2

// miscelanious
#define PWM_RESOLUTION 255

extern gameMap g_map;

class gameMenu
{
private:
    bool m_inSubmenu = false;
    int m_state = MENU_IN_START_GAME;
    int m_settingsState = RETURN_FROM_SETTINGS;

    bool m_changedState = true;
    unsigned long m_lastCycleTime = 0;

    bool m_showedIntroMessage = false;
    unsigned long m_introMessageTime = 0;

    bool m_showEndMessage = false;
    unsigned long m_endMessageTime = 0;

    unsigned long m_lastContrastChange = 0;
    unsigned long m_lcdScrollChange = 0;
    unsigned long m_lastMatrixBrightnessChange = 0;

    // menu variables:
    bool m_showAboutText = false;

    byte m_lcdContrast = 100;
    inputHwControl m_hwCtrl;
    // const int RESET = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
    LiquidCrystal m_lcd = LiquidCrystal(RESET, ENABLE, DATA4, DATA5, DATA6, DATA7);

    void changeState(int &p_state, int p_newState)
    {
        p_state = p_newState;
        
        m_lcd.clear();
        m_changedState = true;
    }

    bool doIntroMessageSequence()
    {
        // don't enter if intro message has been shown
        if(m_showedIntroMessage)
        {
            return false;
        }

        if(millis() - m_introMessageTime < INTRO_MESSAGE_MILLIS)
        {
            if(m_changedState)
            {
                displayStartMessage();
                m_changedState = false;
            }
        }
        else if (!m_showedIntroMessage)
        {
            m_showedIntroMessage = true;
            m_lcd.clear();
            m_changedState = true;
            return false;
        }

        // bypass intro message mechanism
        int xCommand, yCommand;
        if(m_hwCtrl.pressedButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
        {
            m_showedIntroMessage = true;
            m_lcd.clear();
            m_changedState = true;
            return false;
        }

        return true;
    }

    bool doEndMessageSequence()
    {
        // don't enter if intro message has been shown
        if(!m_showEndMessage)
        {
            return false;
        }

        if(millis() - m_endMessageTime < END_MESSAGE_MILLIS)
        {
            if(m_changedState)
            {
                displayEndMessage();
                m_changedState = false;
            }
        }
        else if (m_showEndMessage)
        {
            m_showEndMessage = false;
            m_lcd.clear();
            m_changedState = true;
            return false;
        }

        // bypass end message mechanism
        int xCommand, yCommand;
        if(m_hwCtrl.pressedButton() || m_hwCtrl.joystickDetected(xCommand, yCommand))
        {
            m_showEndMessage = false;
            m_lcd.clear();
            m_changedState = true;
            return false;
        }
        return true;
    }

    void displayStartMessage()
    {
        m_lcd.setCursor(SECOND_LCD_COL, FIRST_LCD_ROW);
        m_lcd.print(F("Welcome to ..."));
        m_lcd.setCursor(SECOND_LCD_COL, SECOND_LCD_ROW);
        m_lcd.print(F("ROCKET  COWBOY"));
    }
    
    void displayEndMessage()
    {
        m_lcd.setCursor(FIRST_LCD_COL, FIRST_LCD_ROW);
        m_lcd.print(F("Well done COWBOY"));
        m_lcd.setCursor(SECOND_LCD_COL, SECOND_LCD_ROW);
        m_lcd.print(F("keep  slingin'"));   
    }

public:
    gameMenu()
    {
        m_lcd.begin(LCD_COLS, LCD_ROWS);
        m_lcd.clear();
        analogWrite(LCD_CONTRAST, m_lcdContrast);
    }

    void printEndMessage()
    {
        m_showEndMessage = true;
        m_endMessageTime = millis();
    }

    void keepStateInBounds(int &p_state, const int p_lowerBound, const int p_upperBound)
    {
        if(p_state > p_upperBound)
        {
            p_state = p_lowerBound;
        }

        if(p_state < p_lowerBound)
        {
            p_state = p_upperBound;
        }
    }
    
    void setStateToDefault()
    {
        changeState(m_state, MENU_IN_START_GAME);
    }

    void refreshMenuVariables()
    {
        m_showAboutText = false;
    }

    void goToNextMenuOption(int &p_currentState, const int p_lowerBound, const int p_upperBound)
    {
        int xJsCommand, yJsCommand;
        if(m_hwCtrl.joystickDetected(xJsCommand, yJsCommand))
        {
            if(millis() - m_lastCycleTime > CYCLE_DELAY_MILLIS)
            {
                // i want to only account for ups and downs, no lefts or rights
                if(absolute(JS_DEFAULT_VALUE - xJsCommand) < absolute(JS_DEFAULT_VALUE - yJsCommand))
                {
                    return;
                }

                m_lastCycleTime = millis();

                if(xJsCommand < MIN_JS_THRESHOLD)
                {
                    p_currentState --;
                }
                else if(xJsCommand > MIN_JS_THRESHOLD)
                {
                    p_currentState ++;
                }
        
                keepStateInBounds(p_currentState, p_lowerBound, p_upperBound);

                m_lcd.clear();
                m_changedState = true;
                refreshMenuVariables();
            }
        }
    }

    void printHashesLCD(int p_fillAmount)
    {
        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);

        int percent = map(p_fillAmount, 0, PWM_RESOLUTION, LCD_COLS, 0);
        for(int i = 0; i < LCD_COLS; i++)
        {
            if(i < percent)
            {
                m_lcd.print("#");
            }
            else
            {
                m_lcd.print(" ");
            }
        }
    }

    void printHashesForMatrix(int p_fillAmount)
    {
        m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_ROW);

        for(int i = 0; i < LCD_COLS; i++)
        {
            if(i < p_fillAmount)
            {
                m_lcd.print("#");
            }
            else
            {
                m_lcd.print(" ");
            }
        }
    }

    void goToSettingsMenu()
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

    int menuSequence()
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
};

#endif