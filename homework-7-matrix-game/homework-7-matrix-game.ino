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

// matrix occupation related
#define MAP_EMPTY 0
#define MAP_PLAYER 15

#define MAP_BULLET_LEFT 16
#define MAP_BULLET_RIGHT 17
#define MAP_BULLET_UP 18
#define MAP_BULLET_DOWN 19

#define MAP_WALL 20

class gameMap
{   
private:
    LedControl p_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
    byte p_matrixBrightness = 1;

    byte matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}  
    };

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
        for (int row = 0; row < MATRIX_SIZE; row++) 
        {
            for (int col = 0; col < MATRIX_SIZE; col++) 
            {
                p_ledControl.setLed(MATRIX_ADDRESS, row, col, matrix[row][col]);  // set each led individually
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