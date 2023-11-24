#ifndef BULLET_H
#define BULLET_H

#include "matrixEntity.h"

#define DEFAULT_RANGE 3

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2 
#define DIRECTION_RIGHT 3

class bullet : public matrixEntity
{
private:
    byte m_direction;
    int m_rangeLeft;

public:
    bullet(int p_xPos, int p_yPos, byte p_direction, int p_rangeLeft = DEFAULT_RANGE);

    void updatePosition() override;

    bool hasRange();

    ~bullet();
};

#endif