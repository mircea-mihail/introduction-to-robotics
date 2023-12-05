#include "gameMap.h"
#include "matrixEntity.h"
#include "bullet.h"
#include "bulletList.h"
#include "player.h"
#include "score.h"
#include "gameMenu.h"
#include "inputHwControl.h"

gameMap g_map;
player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);
bulletList g_bulletList;
score g_score;
inputHwControl g_hwCtrl;

byte g_gameState;

// frame display
bool g_finishedAnimiation = false;
unsigned long g_timeForLastFrame = 0;
unsigned long g_timeForBulletUpdate = 0;

void initAllHw()
{
    // button and js
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);   

    // matrix
    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    // LCD
    pinMode(RESET, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(DATA4, OUTPUT);
    pinMode(DATA5, OUTPUT);
    pinMode(DATA6, OUTPUT);
    pinMode(DATA7, OUTPUT);

    // brightness sensor
    pinMode(BRIGHTNESS_PIN, INPUT);
    
}

// call this when starting a new level
void startLevelSequence()
{
    g_gameState = IN_GAME;     
    g_map.refreshAnimationValues();               
    g_map.generateMap();
    g_score.startCounting();
    g_score.printHighScores();
    
    g_player1.resetPowerUps();
    g_player1.goToDefaultPosition();

    // g_map.printEmptyMatrix();
    // g_score.clearScores();
}

// adjust brightness using the sensor
void adjustBrightness()
{
    int outsideBrightness = g_hwCtrl.getBrightness();
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
        // Serial.println(newBrightness);
        g_map.setMatrixBrightness(newBrightness);
    } 
}   

void doInGameRoutine()
{
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
}

void doInStartAnimationRoutine()
{
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
}

void doInWinningStateRoutine()
{
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
}

void setup()
{
    Serial.begin(115200);
    initAllHw();

    g_gameState = IN_START_ANIMATION;
    g_map.initMatrix();
}

void loop() 
{
    adjustBrightness();

    switch(g_gameState)
    {
        case IN_START_ANIMATION:
            doInStartAnimationRoutine();
            break;
        
        case IN_GAME:
            doInGameRoutine();
            break;

        case WON:
            doInWinningStateRoutine();
            break;            

        case IN_MENU:
            
            break;

        default:
            break;
    }
} 