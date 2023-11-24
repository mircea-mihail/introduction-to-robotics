#ifndef MATRIX_ENTITY_H
#define MATRIX_ENTITY_H

#include "utility.h"
#include "gameMap.h"

// BULLET 
#define BULLET_MOVE_INTERVAL 300

// joystick 
#define JS_X_PIN A0
#define JS_Y_PIN A1

#define JS_DEFAULT_VALUE 512

#define MIN_JS_THRESHOLD 312
#define MAX_JS_THRESHOLD 712

extern gameMap g_map;

class matrixEntity
{
protected: 
    int m_xPos = 0;
    int m_yPos = 0;

    int m_xNextPos = 0;
    int m_yNextPos = 0;

public:
    static void setupHardware();

    virtual void updatePosition() = 0;

    void initialPositionUpdate(int &p_xPos, int &p_yPos, int p_xCommand, int p_yCommand);

    void dealWithOutOfBounds(int &p_xPos, int &p_yPos);

    void pickAnAxes(int &p_xNextPos, int &p_yNextPos, int p_xPos, int p_yPos, int p_xCommand, int p_yCommand);

    void moveEntityOnMatrix(int p_entityType);
};

#endif