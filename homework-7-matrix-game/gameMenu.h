#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "utility.h"
#include "inputHwControl.h"

// lcd default characteristics
#define LCD_COLS 16
#define LCD_ROWS 2

// printing defines
#define FIRST_LCD_ROW 0
#define SECOND_LCD_ROW 1

#define FIRST_LCD_COL 0
#define SECOND_LCD_COL 1

// menu cycling
#define CYCLE_DELAY_MILLIS 400
#define INTRO_MESSAGE_MILLIS 3000
#define END_MESSAGE_MILLIS 3000

class gameMenu
{
private:
    int m_state;
    
    bool m_changedState = true;
    unsigned long m_lastCycleTime = 0;

    bool m_showedIntroMessage = false;
    unsigned long m_introMessageTime = 0;

    bool m_showEndMessage = false;
    unsigned long m_endMessageTime = 0;

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

    void keepStateInBounds(int &p_state)
    {
        if(p_state > MENU_IN_ABOUT)
        {
            p_state = MENU_IN_START_GAME;
        }

        if(p_state < MENU_IN_START_GAME)
        {
            p_state = MENU_IN_ABOUT;
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

    void goToNextMenuOption()
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
                    m_state --;
                }
                else if(xJsCommand > MIN_JS_THRESHOLD)
                {
                    m_state ++;
                }
        
                keepStateInBounds(m_state);

                m_lcd.clear();
                m_changedState = true;
                refreshMenuVariables();
            }
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

        if(m_state != MENU_IN_GAME)
        {
            goToNextMenuOption();
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
            if(m_changedState)
            {
                m_lcd.print(F("    settings"));
                m_changedState = false;
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
                    m_lcd.print(F("more on github"));
                    m_lcd.setCursor(FIRST_LCD_COL, SECOND_LCD_COL);
                    m_lcd.print(F("/mircea-mihail"));
                    m_changedState = false;
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