#include "bullet.h"

bullet::bullet(int p_xPos, int p_yPos, byte p_direction, bool p_explodingBullets = false ,int p_rangeLeft = DEFAULT_RANGE) : m_direction(p_direction)
{
    m_xPos = p_xPos;
    m_yPos = p_yPos;
    m_rangeLeft = p_rangeLeft;
    m_explodingBullets = p_explodingBullets;
    
    interactWithMapElement(m_xPos, m_yPos);
    g_map.setPositionValue(m_xPos, m_yPos, MAP_BULLET); 
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
        interactWithMapElement(m_xNextPos, m_yNextPos);
        g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_BULLET);

        if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
        {
            g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
        }

        m_xPos = m_xNextPos;
        m_yPos = m_yNextPos;

        m_rangeLeft --;
    }
    else
    {
        m_rangeLeft = -1;
    }
        
}

void bullet::interactWithMapElement(int p_xPos, int p_yPos)
{
    if(g_map.isMapElement(MAP_WALL, p_xPos, p_yPos))
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

bool bullet::isExplodingType()
{
    return m_explodingBullets;
}

void bullet::explodeBullet()
{
    if(m_explodingBullets)
    {
        // first time the bullet is in this stage, the squares are set
        if(!m_hasWaitedATick)
        {
            int explosionLeft = m_xPos - EXPLOSION_RADIUS;
            int explosionRight = m_xPos + EXPLOSION_RADIUS;
            int explosionTop = m_yPos - EXPLOSION_RADIUS;
            int explosionBottom = m_yPos + EXPLOSION_RADIUS;

            dealWithOutOfBounds(explosionLeft, explosionTop);
            dealWithOutOfBounds(explosionRight, explosionBottom);

            g_map.setPositionValue(explosionLeft, m_yPos, MAP_BULLET);
            g_map.setPositionValue(explosionRight, m_yPos, MAP_BULLET);
            g_map.setPositionValue(m_xPos, explosionTop, MAP_BULLET);
            g_map.setPositionValue(m_xPos, explosionBottom, MAP_BULLET);
            
            m_hasWaitedATick = true;
        }
        else
        {
            m_hasExploded = true;
        }
    } // don't set the old position to zero when it explodes (it must flicker because of bullet)
}

bool bullet::hasExploded()
{
    return m_hasExploded;
}


bullet::~bullet()
{
    g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY);
    
    if(m_explodingBullets)
    {
        int explosionLeft = m_xPos - EXPLOSION_RADIUS;
        int explosionRight = m_xPos + EXPLOSION_RADIUS;
        int explosionTop = m_yPos - EXPLOSION_RADIUS;
        int explosionBottom = m_yPos + EXPLOSION_RADIUS;

        dealWithOutOfBounds(explosionLeft, explosionTop);
        dealWithOutOfBounds(explosionRight, explosionBottom);

        g_map.setPositionValue(explosionLeft, m_yPos, MAP_EMPTY);
        g_map.setPositionValue(explosionRight, m_yPos, MAP_EMPTY);
        g_map.setPositionValue(m_xPos, explosionTop, MAP_EMPTY);
        g_map.setPositionValue(m_xPos, explosionBottom, MAP_EMPTY);
    } 
}