#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"

// game states
#define IN_GAME 0
#define IN_MENU 1
#define IN_ANIMATION 2
#define LOST 3
#define WON 4

gameMap g_map;
player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);
bulletList g_bulletList;

byte g_gameState;

void setup()
{
    randomSeed(analogRead(BUTTON_PIN));
    Serial.begin(115200);

    g_player1.setupJoystickAndButton();

    gameMap::setupHardware();
    g_map.initMatrix();
    g_map.generateMap();

    g_gameState = IN_GAME;
}

void loop() {
    switch(g_gameState)
    {
        case IN_GAME:
            g_bulletList.updateBullets();
            g_player1.updatePosition();
            g_player1.shoot();
            g_map.updateDisplay();

            if(g_map.checkWinningCondition())
            {
                g_gameState = WON;
            }

            break;

        case WON:
            g_gameState = IN_GAME;
            break;

        default:
            break;
    }
} 