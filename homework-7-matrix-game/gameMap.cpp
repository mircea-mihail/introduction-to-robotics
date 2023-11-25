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
            break;

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

bool gameMap::isWithinBounds(int p_xPos, int p_yPos)
{
    if(p_xPos >= 0 && p_xPos < MATRIX_SIZE && p_yPos >= 0 && p_yPos < MATRIX_SIZE)
    {
        return true;
    }
    return false;
}

bool gameMap::isMapElement(const byte p_mapElement, int p_xPos, int p_yPos)
{
    if(matrix[p_xPos][p_yPos] == p_mapElement)
    {
        return true;
    }
    return false;
}

void gameMap::generateMap()
{
    randomSeed(analogRead(BUTTON_PIN));
    // randomly generate walls
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {
            long chance = random(0, GRANULARITY_OF_CHANCE);
            if(chance < CHANCE_OF_WALL)
            {
                matrix[row][col] = MAP_WALL; 
            }
        }       
    }
    // clear area around player

    for(int row = MATRIX_MIDDLE - 1; row <= MATRIX_MIDDLE + 1; row++)
    {
        for(int col = MATRIX_MIDDLE - 1; col <= MATRIX_MIDDLE + 1; col++)
        {
            matrix[row][col] = MAP_EMPTY; 
        }
    }
}

bool gameMap::checkWinningCondition()
{
    for(int row = 0; row < MATRIX_SIZE; row++)
    {
        for(int col = 0; col < MATRIX_SIZE; col++)
        {
            if(matrix[row][col] != MAP_BULLET && matrix[row][col] != MAP_PLAYER && matrix[row][col] != MAP_EMPTY)
            {
                return false;
            }
        }       
    }
    return true;
}

void gameMap::printWinningMatrixFrame(byte p_frameToPrint)
{
    for(int row = 0; row < REAL_MATRIX_SIZE; row++)
    {
        for(int col = 0; col < REAL_MATRIX_SIZE; col++)
        {   
            matrix[row][col] = getColValFromByte(m_winningMatrixVector[p_frameToPrint][row]);
        }
    }
}

bool gameMap::printWinningMatrix()
{
    if(millis() - m_lastFrameTime > FRAME_DISPLAY_TIME)
    {
        Serial.print("printing a frame\n");
        m_lastFrameTime = millis();
        printWinningMatrixFrame(m_currentFrameIndex);

        m_currentFrameIndex += 1;

        if(m_currentFrameIndex == WINNING_FRAME_NUMBER)
        {
            m_currentFrameIndex = 0;
            return true;  
        }
    }
    return false;
}

void gameMap::refreshAnimationValues()
{
    m_currentFrameIndex = 0;

}

// for debug
void gameMap::printEmptyMatrix()
{
    for(int row = 0; row < REAL_MATRIX_SIZE; row++)
    {
        for(int col = 0; col < REAL_MATRIX_SIZE; col++)
        {   
            matrix[row][col] = MAP_EMPTY;
        }
    }

    matrix[0][0] = MAP_WALL;
}