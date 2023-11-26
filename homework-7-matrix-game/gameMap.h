#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "LedControl.h" // Include LedControl library for controlling the LED matrix
#include "utility.h"
// #include "player.h"

// bit shifter 
#define DATA_IN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define DRIVER_INDEX 1

// matrix generation
#define GRANULARITY_OF_CHANCE 10
#define CHANCE_OF_WALL 6 // 60%

// matrix animations
#define FRAME_DISPLAY_TIME 300
#define WINNING_FRAME_NUMBER 5

class gameMap
{   
private:
    LedControl p_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
    byte p_matrixBrightness = 1;
    
    unsigned long m_lastPlayerBlink = 0;
    unsigned long m_lastBulletBlink = 0;
    unsigned long m_lastPowerUpBlink = 0;
    
    bool m_blinkPlayer = false;
    bool m_blinkBullet = false;
    bool m_blinkPowerUp = false;

    // for matrix display
    unsigned long m_lastFrameTime = 0;
    byte m_currentFrameIndex = 0;

    unsigned long m_lastBulletMove = 0;

    byte matrix[MATRIX_SIZE][MATRIX_SIZE] = 
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    byte m_winningMatrixVector[MATRIX_SIZE][MATRIX_SIZE] = {
        {
            0b00011000,
            0b00011000,
            0b00111100,
            0b01011010,
            0b01011010,
            0b00111100,
            0b00100100,
            0b00100100
        },
        {
            0b00011000,
            0b00011000,
            0b00111100,
            0b11011011,
            0b00011000,
            0b00111100,
            0b00100100,
            0b00100100
        },
        {
            0b00011000,
            0b10011001,
            0b01111110,
            0b00011000,
            0b00011000,
            0b00111100,
            0b00100100,
            0b00100100
        },
        {
            0b10011001,
            0b10011001,
            0b01111110,
            0b00011000,
            0b00011000,
            0b00111100,
            0b00100100,
            0b00100100
        },
        {
            0b10011001,
            0b10011001,
            0b01111110,
            0b00011000,
            0b00011000,
            0b00111100,
            0b00100100,
            0b00100100
        }
    };

    void displayElement(int realRow, int realCol, int p_row, int p_col);

public:
    static void setupHardware();

    void initMatrix();

    void setPositionValue(int p_xPos, int p_yPos, int p_newValue);
    
    void updateDisplay(int p_xPosPlayer, int p_yPosPlayer);

    bool isWithinBounds(int p_xPos, int p_yPos);

    bool isMapElement(const byte p_mapElement, int p_xPos, int p_yPos);

    void generateMap();

    bool checkWinningCondition();

    void printWinningMatrixFrame(byte p_frameToPrint);
    
    bool printWinningMatrix();

    void refreshAnimationValues();

    void printOnRealMatrix();

    // for debug
    void printEmptyMatrix();
};

#endif