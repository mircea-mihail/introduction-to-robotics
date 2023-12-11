#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "utility.h"
#include "inputHwControl.h"
#include "gameMap.h"
#include "memory.h"

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

    // updates the state and does other changes to prepare the object for the state change
    void changeState(int &p_state, int p_newState);

    // ensures the display of the intro message (when, how, if and for how long)
    bool doIntroMessageSequence();

    // ensures the display of the end game message (when, how, if and for how long)
    bool doEndMessageSequence();

    // only does the printing of the acutal intro words
    void displayStartMessage();
    
    // only does the printing of the acutal end game words
    void displayEndMessage();

    // makes sure the state given is within bounds and if not, modifies it to be
    void keepStateInBounds(int &p_state, const int p_lowerBound, const int p_upperBound);

    // the settings menu logic that orchestrates the changes between settings-menu options
    void goToSettingsMenu();

    // changes the current menu option by increasing/decreasing the menu state according to joystick input 
    void goToNextMenuOption(int &p_currentState, const int p_lowerBound, const int p_upperBound);

    // used to refresh menu variables when going from a state to another
    void refreshMenuVariables();

    // used to print percent hashes showing LCD related info in a graphical manner
    void printHashesLCD(int p_fillAmount);

    // used to print percent hashes showing Matrix related info in a graphical manner
    void printHashesForMatrix(int p_fillAmount);

public:
    // constructor, does the initializations and setups for the lcd
    gameMenu();

    // the main menu logic called in the main .ino file that ensures the changes between menu options
    int menuSequence();

    // initiates the printing of the main message by setting a variable to true
    void printEndMessage();
    
    // used to reset the menu state to begin printing all the settings options after finishing a game
    void setStateToDefault();
};

#endif