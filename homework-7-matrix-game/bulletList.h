#ifndef BULLET_LIST_H
#define BULLET_LIST_H

#include "bullet.h"

struct bulletNode
{
    bullet *m_bullet;
    bulletNode *m_nextBulletNode;
    bulletNode *m_prevBulletNode;

    bulletNode(bullet *p_newBullet, bulletNode *p_nextBulletNode, bulletNode *p_prevBulletNode);

    ~bulletNode();
};

class bulletList
{
    bulletNode *m_firstBulletNode;
    bulletNode *m_lastBulletNode;

    unsigned long m_lastBulletUpdate = 0;

public:
    void deleteBulletNode(bulletNode *p_bulletNode);  

    void addBulletNode(bullet *p_newBullet);

    void updateBullets();

    void setLastBulletUpdate();

};


#endif