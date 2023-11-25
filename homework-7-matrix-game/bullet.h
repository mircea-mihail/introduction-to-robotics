#ifndef BULLET_H
#define BULLET_H

#include "matrixEntity.h"

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