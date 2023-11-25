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

void player::setDirection(int p_xNextPos, int p_yNextPos, int p_xPos, int p_yPos)
{
    if(p_xNextPos != p_xPos && p_yNextPos == p_yPos)
    {
        m_direction = ((p_xNextPos - p_xPos > 0) ? DIRECTION_DOWN : DIRECTION_UP);
    }
    
    if(p_xNextPos == p_xPos && p_yNextPos != p_yPos)
    {
        m_direction = ((p_yNextPos - p_yPos > 0) ? DIRECTION_RIGHT : DIRECTION_LEFT);
    }
}

bool player::pulledTrigger()
{
    unsigned long time = millis();
    int state = !digitalRead(BUTTON_PIN);

    // if the button has a constant state
    if(state == m_btn.m_prevState)
    {
        // if the constant state has been kept for a while
        if(time - m_btn.m_prevTime > DEBOUNCE_TIME && m_btn.m_prevCountedState != state)
        {
            m_btn.m_prevCountedState = state;

            if(state == HIGH)
            {
                m_btn.m_prevState = state;
                return true;
            }
        }
    }
    else
    {
        m_btn.m_prevTime = time;
    }

    m_btn.m_prevState = state;
    
    return false;
}

void player::getBulletPlacement(int &p_xPos, int &p_yPos)
{
    switch (m_direction)
    {
        case DIRECTION_UP:
            p_xPos -= 1;
            break;

        case DIRECTION_DOWN:
            p_xPos += 1;
            break;

        case DIRECTION_LEFT:
            p_yPos -= 1;
            break;

        case DIRECTION_RIGHT:
            p_yPos += 1;
            break;

        default:
            break;
    }
}

//////////////// public

player::player(int p_xPos, int p_yPos)
{
    m_direction = DIRECTION_RIGHT;
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER); 
}

void player::setupJoystickAndButton()
{
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);   
}

void player::updatePosition()  
{
    int xCommand = analogRead(JS_X_PIN);
    int yCommand = analogRead(JS_Y_PIN);

    g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER);

    if(joystickDetected(xCommand, yCommand))
    {
        storeMovement(xCommand, yCommand);
        
        if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
        {
            dealWithOutOfBounds(m_xNextPos, m_yNextPos);
            setDirection(m_xNextPos, m_yNextPos, m_xPos, m_yPos);

            g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_PLAYER);

            // set old position to zero 
            if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
            {
                g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
                m_xPos = m_xNextPos;
                m_yPos = m_yNextPos;
            }

            m_lastMoved = millis();
        }
    }
}

void player::shoot()
{
    if(millis() - m_lastShot > SHOOTING_COOLDOWN && pulledTrigger())
    {
        int xPos = m_xPos, yPos = m_yPos;

        getBulletPlacement(xPos, yPos);
        g_bulletList.addBulletNode(new bullet(xPos, yPos, m_direction));

        m_lastShot = millis();
    }
}