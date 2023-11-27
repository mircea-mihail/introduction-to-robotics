#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"
#include "score.h"
#include "peripherals.h"

gameMap g_map;
player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);
bulletList g_bulletList;
score g_score;
peripherals g_periph;

byte g_gameState;

// frame display
bool g_finishedAnimiation = false;
unsigned long g_timeForLastFrame = 0;
unsigned long g_timeForBulletUpdate = 0;

// call this when starting a new level
void startLevelSequence()
{
    g_gameState = IN_GAME;     
    g_map.refreshAnimationValues();               
    g_map.generateMap();
    g_score.startCounting();
    
    g_player1.resetPowerUps();
    g_player1.goToDefaultPosition();

    // g_map.printEmptyMatrix();
    // g_score.clearScores();
}

// adjust brightness using the sensor
void adjustBrightness()
{
    int outsideBrightness = g_periph.getBrightness();
    if(outsideBrightness < MIN_BRIGHTNESS_INPUT)
    {
        g_map.setMatrixBrightness(MIN_MATRIX_BRIGHTNESS);
    }  
    else if(outsideBrightness > MAX_BRIGHTNESS_INPUT)
    {
        g_map.setMatrixBrightness(MAX_MATRIX_BRIGHTNESS);
    } 
    else
    {
        int newBrightness = map(outsideBrightness, MIN_BRIGHTNESS_INPUT, MAX_BRIGHTNESS_INPUT, MIN_MATRIX_BRIGHTNESS, MAX_MATRIX_BRIGHTNESS);
        Serial.println(newBrightness);
        g_map.setMatrixBrightness(newBrightness);
    } 
}   

void setup()
{
    Serial.begin(115200);

    g_player1.setupJoystickAndButton();
    gameMap::setupHardware();

    g_gameState = IN_START_ANIMATION;
    g_map.initMatrix();
}

void loop() 
{
    adjustBrightness();

    switch(g_gameState)
    {
        case IN_START_ANIMATION:
            g_map.updateDisplay(0, 0);

            if(g_map.printStartGameMatrixAnimation())
            {
                if(g_timeForLastFrame == DEFAULT_TIME_VAL)
                {
                    g_timeForLastFrame = millis();
                }
                g_finishedAnimiation = true;   
            }
        
            if(millis() - g_timeForLastFrame > GAME_START_FRAME_DISPLAY_TIME && g_finishedAnimiation)
            {
                g_timeForLastFrame = DEFAULT_TIME_VAL;
                g_finishedAnimiation = false;

                g_gameState = IN_GAME;
                
                startLevelSequence();
            }

            break;
        
        case IN_GAME:
            g_bulletList.updateBullets();
            g_player1.updatePosition();
            g_player1.shoot();

            int p_xPosPlayer, p_yPosPlayer;
            g_player1.getCoordonates(p_xPosPlayer, p_yPosPlayer);

            g_map.updateDisplay(p_xPosPlayer, p_yPosPlayer);

            if(g_map.checkWinningCondition())
            {
                if(g_timeForBulletUpdate == DEFAULT_TIME_VAL)
                {
                    g_timeForBulletUpdate = millis();
                }
                if(millis() - g_timeForBulletUpdate > WINNING_FRAME_DISPLAY_TIME)
                {
                    g_gameState = WON;
                    g_timeForBulletUpdate = DEFAULT_TIME_VAL;
                    Serial.print("Your score: ");
                    Serial.println(g_score.stopCounting());
                }
            }

            break;

        case WON:
            if(g_map.printWinningMatrixAnimation())
            {
                if(g_timeForLastFrame == DEFAULT_TIME_VAL)
                {
                    g_timeForLastFrame = millis();
                }
                g_finishedAnimiation = true;
            }
            
            if(millis() - g_timeForLastFrame > WINNING_FRAME_DISPLAY_TIME && g_finishedAnimiation)
            {
                g_timeForLastFrame = DEFAULT_TIME_VAL;
                g_finishedAnimiation = false;

                g_gameState = IN_START_ANIMATION;
            }

            g_map.printOnRealMatrix();
            break;

        case IN_PAUSE:
            

        default:
            break;
    }
} 