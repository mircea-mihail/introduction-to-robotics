#include "LedControl.h" // Include LedControl library for controlling the LED matrix

//////////////////////// bit shifter 
#define DATA_IN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define DRIVER_INDEX 1

//////////////////////// joystick 
#define JS_X_PIN A0
#define JS_Y_PIN A1

#define JS_DEFAULT_VALUE 512

const int g_minJsThreshold = 312;
const int g_maxJsThreshold = 712;

//////////////////////// utility
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

//////////////////////// matrix related
#define MATRIX_ADDRESS 0

LedControl g_ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN, DRIVER_INDEX);
byte g_matrixBrightness = 1;

const byte matrixSize = 8;

byte matrix[matrixSize][matrixSize] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}  
};

//////////////////////// player related
#define PLAYER_MOVEMENT_MILLIS 200

class player
{
private:
    int m_xPos = 0;
    int m_yPos = 0;
    int m_xLastPos = 0;
    int m_yLastPos = 0;
    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

public:
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
            p_xPos = matrixSize - 1;
        }
        if(p_yPos < 0)
        {
            p_yPos = matrixSize - 1;
        }
        if(p_xPos >= matrixSize)
        {
            p_xPos = 0;
        }
        if(p_yPos >= matrixSize)
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
                p_yPos = m_yLastPos;
            }
            else
            {
                p_xPos = p_xLastPos;
            }
        }
    }

    void updatePositions() 
    {
        // storeMovement();

        if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
        {
            int xCommand = analogRead(JS_X_PIN);
            int yCommand = analogRead(JS_Y_PIN);

            m_xLastPos = m_xPos;
            m_yLastPos = m_yPos;

            initialPositionUpdate(m_xPos, m_yPos, xCommand, yCommand);
            dealWithOutOfBounds(m_xPos, m_yPos);
            pickAnAxes(m_xPos, m_yPos, m_xLastPos, m_yLastPos, xCommand, yCommand);

            matrix[m_xPos][m_yPos] = 1;
            if(m_xPos != m_xLastPos || m_yPos != m_yLastPos)
            {
                matrix[m_xLastPos][m_yLastPos] = 0; 
            }

            m_lastMoved = millis();
        }
    }

};

player g_player1;

// Array representing each row of the LED matrix as a byte

void setup()
{
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);

    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    Serial.begin(115200);
    // the zero refers to the MAX7219 number, it is zero for 1 chip
    g_ledControl.shutdown(MATRIX_ADDRESS, false); // turn off power saving, enables display
    g_ledControl.setIntensity(MATRIX_ADDRESS, g_matrixBrightness); // sets brightness (0~15 possible values)
    g_ledControl.clearDisplay(MATRIX_ADDRESS); // Clear the matrix display

    g_player1.updatePositions();
}

void loop() {
    g_player1.updatePositions();

    updateMatrix();
    // Serial.print("loop");
} 

void updateMatrix() {
    for (int row = 0; row < matrixSize; row++) 
    {
        for (int col = 0; col < matrixSize; col++) 
        {
            g_ledControl.setLed(0, row, col, matrix[row][col]);  // set each led individually
        }
    }
}