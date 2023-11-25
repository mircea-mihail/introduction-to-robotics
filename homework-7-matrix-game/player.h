#ifndef PLAYER_H
#define PLAYER_H

#include "matrixEntity.h"
#include "bulletList.h"
#include "gameMap.h"

// player pins
#define JS_X_PIN A0
#define JS_Y_PIN A1
#define BUTTON_PIN 6

// button
#define DEBOUNCE_TIME 50

extern bulletList g_bulletList;
extern gameMap g_map;

struct button
{
    int m_prevState = LOW;
    int m_prevCountedState = LOW;
    unsigned long m_prevTime = 0;
};

class player : public matrixEntity
{
    byte m_direction;
    button m_btn;
    unsigned long m_lastShot = 0;

private:
    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

    void storeMovement(int p_xCommand, int p_yCommand);
    
    bool joystickDetected(int p_xCommand, int p_yCommand);

    void setDirection(int p_xNextPos, int p_yNextPos, int p_xPos, int p_yPos);

    bool pulledTrigger();

    void getBulletPlacement(int &p_xPos, int &p_yPos);

public:
    player(int p_xPos, int p_yPos);

    void setupJoystickAndButton();

    void updatePosition() override;

    void shoot();
};

#endif