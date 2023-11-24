#include "player.h"

//////////////// private
void player::storeMovement(int p_xCommand, int p_yCommand)
{
    m_xNextPos = m_xPos;
    m_yNextPos = m_yPos;

    initialPositionUpdate(m_xNextPos, m_yNextPos, p_xCommand, p_yCommand);
    pickAnAxes(m_xNextPos, m_yNextPos, m_xPos, m_yPos, p_xCommand, p_yCommand);
}

bool player::joystickDetected(int p_xCommand, int p_yCommand)
{
    if(p_xCommand < MIN_JS_THRESHOLD || p_xCommand > MAX_JS_THRESHOLD || p_yCommand < MIN_JS_THRESHOLD || p_yCommand > MAX_JS_THRESHOLD)
    {
        return true;
    }
    return false;
}

//////////////// public

player::player(int p_xPos, int p_yPos)
{
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER); 
}

void player::updatePosition()  
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    if(joystickDetected(xCommand, yCommand))
    {
        storeMovement(xCommand, yCommand);
        
        if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
        {
            dealWithOutOfBounds(m_xNextPos, m_yNextPos);

            moveEntityOnMatrix(MAP_PLAYER);

            m_lastMoved = millis();
        }
    }
}