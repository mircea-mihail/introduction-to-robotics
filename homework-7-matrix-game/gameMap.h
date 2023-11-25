#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "LedControl.h" // Include LedControl library for controlling the LED matrix
#include "utility.h"

// bit shifter 
#define DATA_IN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define DRIVER_INDEX 1

class gameMap
{   
private:
    LedControl p_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
    byte p_matrixBrightness = 1;
    
    unsigned long m_lastPlayerBlink = 0;
    unsigned long m_lastBulletBlink = 0;
    
    bool m_blinkPlayer = false;
    bool m_blinkBullet = false;

    unsigned long m_lastBulletMove = 0;

    byte matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, MAP_WALL, MAP_WALL, MAP_WALL, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}  
    };


    void displayElement(int p_row, int p_col);

public:
    static void setupHardware();

    void initMatrix();

    void setPositionValue(int p_xPos, int p_yPos, int p_newValue);
    
    void updateDisplay();

    bool isWithinBounds(int p_xPos, int p_yPos);

    bool isMapElement(const byte p_mapElement, int p_xPos, int p_yPos);
};

#endif