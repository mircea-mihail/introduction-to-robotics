#include "bulletList.h"

//////////////// bullet Node:

bulletNode::bulletNode(bullet *p_newBullet, bulletNode *p_nextBulletNode, bulletNode *p_prevBulletNode)
{
    m_bullet = p_newBullet;
    m_nextBulletNode = p_nextBulletNode;
    m_prevBulletNode = p_prevBulletNode;
    Serial.print("node constructor\n");
}

bulletNode::~bulletNode()
{
    delete m_bullet;
    Serial.print("node destructor\n");
}

//////////////// bullet List:

void bulletList::deleteBulletNode(bulletNode *p_bulletNode)
{
    // if this is the only bullet, delete it and make the first bullet null
    if(p_bulletNode->m_prevBulletNode == NULL && p_bulletNode->m_nextBulletNode == NULL)
    {
        m_firstBulletNode = NULL;
        m_lastBulletNode = NULL;
    }
    else if(p_bulletNode->m_prevBulletNode == NULL)
    {
        m_firstBulletNode = p_bulletNode->m_nextBulletNode;
        p_bulletNode->m_nextBulletNode->m_prevBulletNode = NULL;
    }
    else if(p_bulletNode->m_nextBulletNode == NULL)
    {
        m_lastBulletNode = p_bulletNode->m_prevBulletNode;
        p_bulletNode->m_prevBulletNode->m_nextBulletNode = NULL;
    }
    else
    {
        p_bulletNode->m_prevBulletNode->m_nextBulletNode = p_bulletNode->m_nextBulletNode;
        p_bulletNode->m_nextBulletNode->m_prevBulletNode = p_bulletNode->m_prevBulletNode;
    }

    delete p_bulletNode;
}    

void bulletList::addBulletNode(bullet *p_newBullet)
{
    if(m_lastBulletNode == NULL)
    {
        m_lastBulletNode = new bulletNode(p_newBullet, NULL, NULL);
        m_firstBulletNode = m_lastBulletNode;
    }
    else
    {
        m_lastBulletNode->m_nextBulletNode = new bulletNode(p_newBullet, NULL, m_lastBulletNode);
        m_lastBulletNode = m_lastBulletNode->m_nextBulletNode;
    }
}

void bulletList::updateBullets()
{
    if(millis() - m_lastBulletUpdate > BULLET_TICK_RATE)
    {
        bulletNode *currentNode = m_firstBulletNode;
        while(currentNode != NULL)
        {
            if(!currentNode->m_bullet->hasRange())
            {
                bulletNode *nodeToDelete = currentNode; 
                currentNode = currentNode->m_nextBulletNode;
                deleteBulletNode(nodeToDelete);

            }
            else
            {
                currentNode->m_bullet->updatePosition();
                currentNode = currentNode->m_nextBulletNode;
            }
        }
        m_lastBulletUpdate = millis();
    }
}

void bulletList::setLastBulletUpdate()
{
    m_lastBulletUpdate = millis();
}