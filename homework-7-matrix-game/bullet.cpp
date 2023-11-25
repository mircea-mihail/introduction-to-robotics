#include "bullet.h"

bullet::bullet(int p_xPos, int p_yPos, byte p_direction, int p_rangeLeft = DEFAULT_RANGE) : m_direction(p_direction)
{
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    g_map.setPositionValue(m_xPos, m_yPos, MAP_BULLET); 
    m_rangeLeft = p_rangeLeft;
}

void bullet::updatePosition()
{
    m_xNextPos = m_xPos;
    m_yNextPos = m_yPos;

    switch(m_direction)
    {
        case(DIRECTION_UP):
            m_xNextPos -= 1;
            break;
        
        case(DIRECTION_DOWN):
            m_xNextPos += 1;
            break;

        case(DIRECTION_LEFT):
            m_yNextPos -= 1;
            break;

        case(DIRECTION_RIGHT):
            m_yNextPos += 1;
            break;

        default:
            return;
            break;
    }

    dealWithOutOfBounds(m_xNextPos, m_yNextPos);
    
    if(g_map.isWithinBounds(m_xNextPos, m_yNextPos))
    {
        g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_BULLET);

        // set old position to zero 
        if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
        {
            g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
            m_xPos = m_xNextPos;
            m_yPos = m_yNextPos;
        }

        m_rangeLeft --;
    }
    else
    {
        m_rangeLeft = -1;
    }
        
}

bool bullet::hasRange()
{
    if(m_rangeLeft > 0)
    {
        return true;
    }
    return false;

}

bullet::~bullet()
{
    g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
}