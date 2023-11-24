#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"

#define IN_GAME 0
#define IN_MENU 1
#define IN_ANIMATION 2

gameMap g_map;

player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);

bulletList g_bulletList;

byte g_gameState;

void setup()
{
    Serial.begin(115200);

    matrixEntity::setupHardware();
    gameMap::setupHardware();

    g_map.initMatrix();

    g_gameState = IN_GAME;

    g_bulletList.addBulletNode(new bullet(3, 0, DIRECTION_LEFT, 1));
    g_bulletList.addBulletNode(new bullet(3, 1, DIRECTION_LEFT, 2));
    g_bulletList.addBulletNode(new bullet(3, 2, DIRECTION_LEFT, 1));
    g_bulletList.addBulletNode(new bullet(3, 3, DIRECTION_LEFT, 6));
}

void loop() {
    if(g_gameState == IN_GAME)
    {
        g_bulletList.updateBullets();
        g_player1.updatePosition();
        g_map.updateDisplay();
    }
} 