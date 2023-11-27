#ifndef BULLET_H
#define BULLET_H

#include "matrixEntity.h"
#include "gameMap.h"

extern gameMap g_map;

#define EXPLOSION_RADIUS 1

// shot by player 
class bullet : public matrixEntity
{
private:
    byte m_direction;
    int m_rangeLeft;
    bool m_explodingBullets = false;
    bool m_hasWaitedATick = false;
    bool m_hasExploded = false;

public:
    // initialises the bullet members and interacts with the map element on the spawning position
    bullet(int p_xPos, int p_yPos, byte p_direction, bool p_explodingBullets = false, int p_rangeLeft = DEFAULT_RANGE);

    // updates the bullet position using the direction it is going in
    void updatePosition() override;

    // checks if the range left is > 0
    bool hasRange();

    // decides what to do next taking into account the new given position
    void interactWithMapElement(int p_xPos, int p_yPos);

    // checks if the bullet is exploding type or regular type
    bool isExplodingType();

    // deals with the explosion of the bullet
    void explodeBullet();

    // checks if the bullet has exploded
    bool hasExploded();

    // deletes the bullet and the explosion area if the bullet is the exploding type
    ~bullet();
};

#endif