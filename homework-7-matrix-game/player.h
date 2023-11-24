#ifndef PLAYER_H
#define PLAYER_H

#include "matrixEntity.h"

#define PLAYER_MOVEMENT_MILLIS 200

class player : public matrixEntity
{
private:
    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

    void storeMovement(int p_xCommand, int p_yCommand);
    
    bool joystickDetected(int p_xCommand, int p_yCommand);

public:
    player(int p_xPos, int p_yPos);

    void updatePosition() override ;
};

#endif