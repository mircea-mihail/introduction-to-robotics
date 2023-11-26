#ifndef BULLET_H
#define BULLET_H

#include "matrixEntity.h"
#include "gameMap.h"

extern gameMap g_map;

#define EXPLOSION_RADIUS 1

class bullet : public matrixEntity
{
private:
    byte m_direction;
    int m_rangeLeft;
    bool m_explodingBullets = false;
    bool m_hasWaitedATick = false;
    bool m_hasExploded = false;

public:
    bullet(int p_xPos, int p_yPos, byte p_direction, bool p_explodingBullets = false, int p_rangeLeft = DEFAULT_RANGE);

    void updatePosition() override;

    bool hasRange();

    void interactWithMapElement(int p_xPos, int p_yPos);

    bool isExplodingType();

    void explodeBullet();

    bool hasExploded();

    ~bullet();
};

#endif