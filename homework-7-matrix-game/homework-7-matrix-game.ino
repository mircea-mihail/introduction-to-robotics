#include "LedControl.h" // Include LedControl library for controlling the LED matrix

//////////////////////////////////////////////// bit shifter 
#define DATA_IN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define DRIVER_INDEX 1

//////////////////////////////////////////////// joystick 
#define JS_X_PIN A0
#define JS_Y_PIN A1

#define JS_DEFAULT_VALUE 512

const int g_minJsThreshold = 312;
const int g_maxJsThreshold = 712;

//////////////////////////////////////////////// utility
inline int absolute(int a) 
{
    if(a < 0)
    {
        return a * -1;
    }
    else 
    {
        return a;
    }
}

//////////////////////////////////////////////// matrix related
#define MATRIX_ADDRESS 0
#define MATRIX_SIZE 8

// display related
#define PLAYER_BLINK_INTERVAL 200
#define BULLET_BLINK_INTERVAL 50
#define BULLET_MOVE_INTERVAL 100

// matrix occupation related
#define MAP_NUMBER_OF_ORIENTATIONS 4
#define MAP_BULLET_RANGE 3
#define MAP_BULLET_FLY_TIME 100

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_POWER_UP 3

// bullets can reach 8 at minimum with min val 12
#define MAP_BULLET_MIN_VAL 12
#define MAP_BULLET_MIN_POSSIBLE_VAL (MAP_BULLET_MIN_VAL - MAP_NUMBER_OF_ORIENTATIONS) 

#define MAP_BULLET_LEFT (MAP_BULLET_MIN_VAL * MAP_BULLET_RANGE)
#define MAP_BULLET_RIGHT (MAP_BULLET_MIN_VAL * MAP_BULLET_RANGE + 1)
#define MAP_BULLET_UP (MAP_BULLET_MIN_VAL * MAP_BULLET_RANGE + 2)
#define MAP_BULLET_DOWN (MAP_BULLET_MIN_VAL * MAP_BULLET_RANGE + 3)

class gameMap
{   
private:
    LedControl p_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
    byte p_matrixBrightness = 1;
    
    unsigned long m_lastPlayerBlink = 0;
    unsigned long m_lastBulletBlink = 0;
    
    bool m_blinkPlayer = false;
    bool m_blinkBullet = false;

    unsigned long m_lastBulletMove = 0;
    bool m_areBulletsOnMap = false;

    byte matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, MAP_BULLET_LEFT, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}  
    };

    void moveBullets()
    {
        for(int row = 0; row < MATRIX_SIZE; row ++)
        {
            for(int col = 0; col < MATRIX_SIZE; col ++)
            {
                if(matrix[row][col] >= MAP_BULLET_MIN_POSSIBLE_VAL)
                {                    
                    if(matrix[row][col] >= MAP_BULLET_MIN_VAL)
                    {
                        matrix[row][col] -= MAP_NUMBER_OF_ORIENTATIONS;
                        //move bullet to next cell
                    }
                    else
                    {
                        matrix[row][col] = MAP_EMPTY;
                    }
                }    
            }
        }
        
    }

    void displayElement(int p_row, int p_col)
    {
        switch(matrix[p_row][p_col])
        {
            case(MAP_PLAYER):
                p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, m_blinkPlayer);

                if(millis() - m_lastPlayerBlink > PLAYER_BLINK_INTERVAL)
                {
                    m_lastPlayerBlink = millis();
                    m_blinkPlayer = !m_blinkPlayer;
                }
                break;

            case(MAP_WALL):
                p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, HIGH);


            case(MAP_EMPTY):
                p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, LOW);
                break;

            default:
                break;
        }
        
        if(matrix[p_row][p_col] >= MAP_BULLET_MIN_POSSIBLE_VAL)
        {
            p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, m_blinkBullet);

            if(millis() - m_lastBulletBlink > BULLET_BLINK_INTERVAL)
            {
                m_lastBulletBlink = millis();
                m_blinkBullet = !m_blinkBullet;
            }

            m_areBulletsOnMap = true;
        }
    }

public:
    void initMatrix()
    {
        p_ledControl.shutdown(MATRIX_ADDRESS, false); // turn off power saving, enables display
        p_ledControl.setIntensity(MATRIX_ADDRESS, p_matrixBrightness); // sets brightness (0~15 possible values)
        p_ledControl.clearDisplay(MATRIX_ADDRESS); // Clear the matrix display
    }

    void setPositionValue(int p_xPos, int p_yPos, int p_newValue)
    {
        matrix[p_xPos][p_yPos] = p_newValue;
    }

    void updateDisplay() 
    {
        // if there's no bullets on map there's no point in checking the whole matrix again
        if(m_areBulletsOnMap && (millis() - m_lastBulletMove > BULLET_MOVE_INTERVAL))
        {
            m_lastBulletMove = millis();
            moveBullets();
        }
        m_areBulletsOnMap = false;

        for (int row = 0; row < MATRIX_SIZE; row++) 
        {
            for (int col = 0; col < MATRIX_SIZE; col++) 
            {
                displayElement(row, col);
            }
        }
    }

};

gameMap g_map;

//////////////////////////////////////////////// player related
#define PLAYER_MOVEMENT_MILLIS 200

class player
{
private:
    int m_xPos = 0;
    int m_yPos = 0;

    int m_xNextPos = 0;
    int m_yNextPos = 0;

    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

    void initialPositionUpdate(int &p_xPos, int &p_yPos, int p_xCommand, int p_yCommand)
    {
        if(p_xCommand < g_minJsThreshold)
        {
            p_xPos = p_xPos - 1;
        }
        if(p_yCommand < g_minJsThreshold)
        {
            p_yPos = p_yPos + 1;
        }
        if(p_xCommand > g_maxJsThreshold)
        {
            p_xPos = p_xPos + 1;
        }
        if(p_yCommand > g_maxJsThreshold)
        {
            p_yPos = p_yPos - 1;
        }
    }

    void dealWithOutOfBounds(int &p_xPos, int &p_yPos)
    {
        if(p_xPos < 0)
        {
            p_xPos = MATRIX_SIZE - 1;
        }
        if(p_yPos < 0)
        {
            p_yPos = MATRIX_SIZE - 1;
        }
        if(p_xPos >= MATRIX_SIZE)
        {
            p_xPos = 0;
        }
        if(p_yPos >= MATRIX_SIZE)
        {
            p_yPos = 0;
        }
    }  

    void pickAnAxes(int &p_xPos, int &p_yPos, int p_xLastPos, int p_yLastPos, int p_xCommand, int p_yCommand)
    {
        if(p_xPos != p_xLastPos && p_yPos != p_yLastPos)
        {
            if(absolute(p_xCommand - JS_DEFAULT_VALUE) > absolute(p_yLastPos - JS_DEFAULT_VALUE))
            {
                p_yPos = p_yLastPos;
            }
            else
            {
                p_xPos = p_xLastPos;
            }
        }
    }

    void storeMovement(int p_xCommand, int p_yCommand)
    {
        m_xNextPos = m_xPos;
        m_yNextPos = m_yPos;

        initialPositionUpdate(m_xNextPos, m_yNextPos, p_xCommand, p_yCommand);
        pickAnAxes(m_xNextPos, m_yNextPos, m_xPos, m_yPos, p_xCommand, p_yCommand);
    }

    bool joystickDetected(int p_xCommand, int p_yCommand)
    {
        if(p_xCommand < g_minJsThreshold || p_xCommand > g_maxJsThreshold || p_yCommand < g_minJsThreshold || p_yCommand > g_maxJsThreshold)
        {
            return true;
        }
        return false;
    }

public:
    void initPosition()
    {
        g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER); 
    }

    void updatePositions() 
    {
        int xCommand = analogRead(JS_X_PIN);
        int yCommand = analogRead(JS_Y_PIN);

        if(joystickDetected(xCommand, yCommand))
        {
            storeMovement(xCommand, yCommand);
            
            if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
            {
                dealWithOutOfBounds(m_xNextPos, m_yNextPos);

                g_map.setPositionValue(m_xNextPos, m_yNextPos, MAP_PLAYER);

                // set old position to zero 
                if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
                {
                    g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
                    m_xPos = m_xNextPos;
                    m_yPos = m_yNextPos;
                }

                m_lastMoved = millis();
            }
        }
    }
};

player g_player1;


void setup()
{
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);

    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    Serial.begin(115200);
    
    g_map.initMatrix();
    g_player1.initPosition();
}

void loop() {

    g_player1.updatePositions();
    
    g_map.updateDisplay();

} 