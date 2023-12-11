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
#define IN_LCD_CONTRAST 1
#define IN_LCD_BRIGHTNESS 2 // always second biggest (for logic purposes)
#define RETURN_FROM_SETTINGS 3 // always the biggest of the three

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
    unsigned long m_lastBrightnessChange = 0;
    unsigned long m_lcdScrollChange = 0;
    unsigned long m_lastMatrixBrightnessChange = 0;

    int m_wallsLeftOnMap = 0;
    // menu variables:
    bool m_showAboutText = false;

    byte m_lcdContrast = PWM_RESOLUTION/2;
    byte m_lcdBrightness = PWM_RESOLUTION/2;
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

public:
    gameMenu()
    {
        analogWrite(LCD_BRIGHTNESS, m_lcdBrightness);
        m_lcd.begin(LCD_COLS, LCD_ROWS);
        m_lcd.clear();
        analogWrite(LCD_CONTRAST, m_lcdContrast);
    }

    void printEndMessage()
    {
        m_showEndMessage = true;
        m_endMessageTime = millis();
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
            if(i <= p_fillAmount)
            {
                m_lcd.print("#");
            }
            else
            {
                m_lcd.print(" ");
            }
        }
    }

    void goToSettingsMenu();

    int menuSequence();
};

#endif