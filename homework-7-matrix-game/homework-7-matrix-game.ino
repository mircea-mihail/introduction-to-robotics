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

// display
#define BULLET_BLINK_INTERVAL 50
#define PLAYER_BLINK_INTERVAL 200

// utility
#define MATRIX_ADDRESS 0
#define MATRIX_SIZE 8
#define MATRIX_MIDDLE (MATRIX_SIZE / 2)

// matrix occupation related
#define MAP_NUMBER_OF_ORIENTATIONS 4
#define MAP_BULLET_RANGE 1

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_POWER_UP 3
#define MAP_BULLET 3

// bullets can reach 8 at minimum with min val 12

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

            case(MAP_BULLET):
                p_ledControl.setLed(MATRIX_ADDRESS, p_row, p_col, m_blinkBullet);

                if(millis() - m_lastBulletBlink > BULLET_BLINK_INTERVAL)
                {
                    m_lastBulletBlink = millis();
                    m_blinkBullet = !m_blinkBullet;
                }

                break;

            default:
                break;
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


//////////////////////////////////////////////// matrix entity related
#define BULLET_MOVE_INTERVAL 300
#define MAP_BULLET_FLY_TIME 100

class matrixEntity
{
protected: 
    int m_xPos = 0;
    int m_yPos = 0;

    int m_xNextPos = 0;
    int m_yNextPos = 0;

public:
    virtual void updatePosition() = 0;

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

    void pickAnAxes(int &p_xNextPos, int &p_yNextPos, int p_xPos, int p_yPos, int p_xCommand, int p_yCommand)
    {
        if(p_xNextPos != p_xPos && p_yNextPos != p_yPos)
        {
            if(absolute(p_xCommand - JS_DEFAULT_VALUE) > absolute(p_yPos - JS_DEFAULT_VALUE))
            {
                p_yNextPos = p_yPos;
            }
            else
            {
                p_xNextPos = p_xPos;
            }
        }
    }

    void moveEntityOnMatrix(int p_entityType)
    {

        g_map.setPositionValue(m_xNextPos, m_yNextPos, p_entityType);

        // set old position to zero 
        if(m_xPos != m_xNextPos || m_yPos != m_yNextPos)
        {
            g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
            m_xPos = m_xNextPos;
            m_yPos = m_yNextPos;
        }
    }
};

////////////////////////////////////////// bullet
#define DEFAULT_RANGE 3

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2 
#define DIRECTION_RIGHT 3

class bullet : public matrixEntity
{
private:
    byte m_direction;
    int m_rangeLeft;

public:
    bullet(int p_xPos, int p_yPos, byte p_direction, int p_rangeLeft = DEFAULT_RANGE) : m_direction(p_direction)
    {
        m_xPos = p_xPos;
        m_yPos = p_yPos;
        g_map.setPositionValue(m_xPos, m_yPos, MAP_BULLET); 
        m_rangeLeft = p_rangeLeft;
    }

    void updatePosition() override
    {
        m_xNextPos = m_xPos;
        m_yNextPos = m_yPos;

        switch(m_direction)
        {
            case(DIRECTION_UP):
                m_yNextPos -= 1;
                break;
            
            case(DIRECTION_DOWN):
                m_yNextPos += 1;
                break;

            case(DIRECTION_LEFT):
                m_xNextPos -= 1;
                break;

            case(DIRECTION_RIGHT):
                m_xNextPos += 1;
                break;

            default:
                return;
                break;
        }

        dealWithOutOfBounds(m_xNextPos, m_yNextPos);
        
        moveEntityOnMatrix(MAP_BULLET);

        m_rangeLeft --;
    }

    bool hasRange()
    {
        if(m_rangeLeft > 0)
        {
            return true;
        }
        return false;

    }

    ~bullet()
    {
        g_map.setPositionValue(m_xPos, m_yPos, MAP_EMPTY); 
    }
};

//////////////////////////////////////////////// player related
#define PLAYER_MOVEMENT_MILLIS 200

class player : public matrixEntity
{
private:
    unsigned long long m_lastMoved = 0; // Tracks the last time the LED moved

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
    player(int p_xPos, int p_yPos)
    {
        m_xPos = p_xPos;
        m_yPos = p_yPos;
        g_map.setPositionValue(m_xPos, m_yPos, MAP_PLAYER); 
    }
    
    void updatePosition() override 
    {
        int xCommand = analogRead(JS_X_PIN);
        int yCommand = analogRead(JS_Y_PIN);

        if(joystickDetected(xCommand, yCommand))
        {
            storeMovement(xCommand, yCommand);
            
            if(millis() - m_lastMoved > PLAYER_MOVEMENT_MILLIS)
            {
                dealWithOutOfBounds(m_xNextPos, m_yNextPos);

                moveEntityOnMatrix(MAP_PLAYER);

                m_lastMoved = millis();
            }
        }
    }
};

player g_player1(MATRIX_MIDDLE, MATRIX_MIDDLE);

// bulletArray()

#define BULLET_TICK_RATE 500

struct bulletNode
{
    bullet *m_bullet;
    bulletNode *m_nextBulletNode;
    bulletNode *m_prevBulletNode;

    bulletNode(bullet *p_newBullet, bulletNode *p_nextBulletNode, bulletNode *p_prevBulletNode)
    {
        m_bullet = p_newBullet;
        m_nextBulletNode = p_nextBulletNode;
        m_prevBulletNode = p_prevBulletNode;
        Serial.print("node constructor\n");
    }

    ~bulletNode()
    {
        delete m_bullet;
        Serial.print("node destructor\n");
    }
};

class bulletList
{
    bulletNode *m_firstBulletNode;
    bulletNode *m_lastBulletNode;

    unsigned long m_lastBulletUpdate = 0;

public:
    void deleteBulletNode(bulletNode *p_bulletNode)
    {
        // if this is the only bullet, delete it and make the first bullet null
        if(p_bulletNode->m_prevBulletNode == NULL && p_bulletNode->m_nextBulletNode == NULL)
        {
            m_firstBulletNode = NULL;
            m_lastBulletNode = NULL;
        }
        else if(p_bulletNode->m_prevBulletNode == NULL)
        {
            m_firstBulletNode = p_bulletNode->m_nextBulletNode;
            p_bulletNode->m_nextBulletNode->m_prevBulletNode = NULL;
        }
        else if(p_bulletNode->m_nextBulletNode == NULL)
        {
            m_lastBulletNode = p_bulletNode->m_prevBulletNode;
            p_bulletNode->m_prevBulletNode->m_nextBulletNode = NULL;
        }
        else
        {
            p_bulletNode->m_prevBulletNode->m_nextBulletNode = p_bulletNode->m_nextBulletNode;
            p_bulletNode->m_nextBulletNode->m_prevBulletNode = p_bulletNode->m_prevBulletNode;
        }

        delete p_bulletNode;
    }    

    void addBulletNode(bullet *p_newBullet)
    {
        if(m_lastBulletNode == NULL)
        {
            m_lastBulletNode = new bulletNode(p_newBullet, NULL, NULL);
            m_firstBulletNode = m_lastBulletNode;
        }
        else
        {
            m_lastBulletNode->m_nextBulletNode = new bulletNode(p_newBullet, NULL, m_lastBulletNode);
            m_lastBulletNode = m_lastBulletNode->m_nextBulletNode;
        }
    }

    void updateBullets()
    {
        if(millis() - m_lastBulletUpdate > BULLET_TICK_RATE)
        {
            bulletNode *currentNode = m_firstBulletNode;
            while(currentNode != NULL)
            {
                if(!currentNode->m_bullet->hasRange())
                {
                    bulletNode *nodeToDelete = currentNode; 
                    currentNode = currentNode->m_nextBulletNode;
                    deleteBulletNode(nodeToDelete);

                }
                else
                {
                    currentNode->m_bullet->updatePosition();
                    currentNode = currentNode->m_nextBulletNode;
                }
            }
            m_lastBulletUpdate = millis();
        }
    }
};

bulletList g_bulletList;

/////////////////////////////// overall game related
#define IN_GAME 0
#define IN_MENU 1
#define IN_ANIMATION 2

byte g_gameState;

void setup()
{
    pinMode(JS_X_PIN, INPUT);
    pinMode(JS_Y_PIN, INPUT);

    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    Serial.begin(115200);
    
    g_map.initMatrix();

    g_gameState = IN_GAME;
}

void loop() {
    if(g_gameState == IN_GAME)
    {
        g_bulletList.updateBullets();
        g_player1.updatePosition();
        g_map.updateDisplay();
    }
} 