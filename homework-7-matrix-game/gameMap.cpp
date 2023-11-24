#include "gameMap.h"

void gameMap::displayElement(int p_row, int p_col)
{
    switch(matrix[p_row][p_col])
    {
        case(MAP_PLAYER):
            p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, m_blinkPlayer);

            if(millis() - m_lastPlayerBlink > PLAYER_BLINK_INTERVAL)
            {
                m_lastPlayerBlink = millis();
                m_blinkPlayer = !m_blinkPlayer;
            }
            break;

        case(MAP_WALL):
            p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, HIGH);


        case(MAP_EMPTY):
            p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, LOW);
            break;

        case(MAP_BULLET):
            p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, m_blinkBullet);

            if(millis() - m_lastBulletBlink > BULLET_BLINK_INTERVAL)
            {
                m_lastBulletBlink = millis();
                m_blinkBullet = !m_blinkBullet;
            }

            break;

        default:
            break;
    }
}

void gameMap::setupHardware()
{
    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);
}

void gameMap::initMatrix()
{
    p_ledControl.shutdown(MATRIX_ADDRESS, false); // turn off power saving, enables display
    p_ledControl.setIntensity(MATRIX_ADDRESS, p_matrixBrightness); // sets brightness (0~15 possible values)
    p_ledControl.clearDisplay(MATRIX_ADDRESS); // Clear the matrix display
}

void gameMap::setPositionValue(int p_xPos, int p_yPos, int p_newValue)
{
    matrix[p_xPos][p_yPos] = p_newValue;
}

void gameMap::updateDisplay() 
{
    for (int row = 0; row < MATRIX_SIZE; row++) 
    {
        for (int col = 0; col < MATRIX_SIZE; col++) 
        {
            displayElement(row, col);
        }
    }
}