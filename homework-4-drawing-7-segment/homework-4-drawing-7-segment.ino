#define PIN_A 12
#define PIN_B 10
#define PIN_C 9
#define PIN_D 8
#define PIN_E 7
#define PIN_F 6
#define PIN_G 5
#define PIN_DP 4

#define JOYSTICK_SW 2
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_LOW_THRESHOLD 410
#define JOYSTICK_HIGH_THRESHOLD 610

#define BLINK_DURATION 500
#define BEGINNING_OF_APP_TIME 0
#define DURATION_OF_DEBOUNCE 50
#define DURATION_FOR_RESET 1000

#define RISING 0
#define FALLING 1

enum sevenSegmentNodes
{
    nodeIdxA,
    nodeIdxB,
    nodeIdxC,
    nodeIdxD,
    nodeIdxE,
    nodeIdxF,
    nodeIdxG,
    nodeIdxDP
};

// What i do is think about each position as a node in a graph with each node having cirten connections
typedef struct ledNode
{
    const int pin;
    bool state;
    ledNode *left;
    ledNode *right;
    ledNode *up;
    ledNode *down;
};

ledNode g_7segGraph[8] = { 
                            PIN_A, 0, 0, 0, 0, 0,
                            PIN_B, 0, 0, 0, 0, 0,
                            PIN_C, 0, 0, 0, 0, 0,
                            PIN_D, 0, 0, 0, 0, 0,
                            PIN_E, 0, 0, 0, 0, 0,
                            PIN_F, 0, 0, 0, 0, 0,
                            PIN_G, 0, 0, 0, 0, 0,
                            PIN_DP, 0, 0, 0, 0, 0
                         };

volatile ledNode *g_currentNode;

unsigned long g_lastBlinktime = 0;
bool g_blinkLedState = 0;

bool g_hasMoved = false;

volatile unsigned long g_lastPressTime = 0;
volatile bool g_fallingOrRising = 0;

void buildNodes()
{
    for (int nodeIdx = nodeIdxA; nodeIdx < nodeIdxDP; nodeIdx++)
    {
        g_7segGraph[nodeIdx].state = LOW;
    }

    // g_7segGraph[nodeIdxA].pin = PIN_A;
    g_7segGraph[nodeIdxA].up = &g_7segGraph[nodeIdxA];
    g_7segGraph[nodeIdxA].down = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxA].left = &g_7segGraph[nodeIdxF];
    g_7segGraph[nodeIdxA].right = &g_7segGraph[nodeIdxB];

    g_7segGraph[nodeIdxB].up = &g_7segGraph[nodeIdxA];
    g_7segGraph[nodeIdxB].down = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxB].left = &g_7segGraph[nodeIdxF];
    g_7segGraph[nodeIdxB].right = &g_7segGraph[nodeIdxB];

    g_7segGraph[nodeIdxC].up = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxC].down = &g_7segGraph[nodeIdxD];
    g_7segGraph[nodeIdxC].left = &g_7segGraph[nodeIdxE];
    g_7segGraph[nodeIdxC].right = &g_7segGraph[nodeIdxDP];

    g_7segGraph[nodeIdxD].up = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxD].down = &g_7segGraph[nodeIdxD];
    g_7segGraph[nodeIdxD].left = &g_7segGraph[nodeIdxE];
    g_7segGraph[nodeIdxD].right = &g_7segGraph[nodeIdxC];

    g_7segGraph[nodeIdxE].up = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxE].down = &g_7segGraph[nodeIdxD];
    g_7segGraph[nodeIdxE].left = &g_7segGraph[nodeIdxE];
    g_7segGraph[nodeIdxE].right = &g_7segGraph[nodeIdxC];

    g_7segGraph[nodeIdxF].up = &g_7segGraph[nodeIdxA];
    g_7segGraph[nodeIdxF].down = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxF].left = &g_7segGraph[nodeIdxF];
    g_7segGraph[nodeIdxF].right = &g_7segGraph[nodeIdxB];

    g_7segGraph[nodeIdxG].up = &g_7segGraph[nodeIdxA];
    g_7segGraph[nodeIdxG].down = &g_7segGraph[nodeIdxD];
    g_7segGraph[nodeIdxG].left = &g_7segGraph[nodeIdxG];
    g_7segGraph[nodeIdxG].right = &g_7segGraph[nodeIdxG];

    g_7segGraph[nodeIdxDP].up = &g_7segGraph[nodeIdxDP];
    g_7segGraph[nodeIdxDP].down = &g_7segGraph[nodeIdxDP];
    g_7segGraph[nodeIdxDP].left = &g_7segGraph[nodeIdxC];
    g_7segGraph[nodeIdxDP].right = &g_7segGraph[nodeIdxDP];
}

void setup()
{
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_C, OUTPUT);
    pinMode(PIN_D, OUTPUT);
    pinMode(PIN_E, OUTPUT);
    pinMode(PIN_F, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_DP, OUTPUT);

    pinMode(JOYSTICK_SW, INPUT_PULLUP);
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);

    buildNodes();
    g_currentNode = &g_7segGraph[nodeIdxDP];

    g_lastBlinktime = millis();

    attachInterrupt(digitalPinToInterrupt(JOYSTICK_SW), changeNodeState, CHANGE);

    Serial.begin(115200);
}

void flashCurrentLed()
{
    if ((millis() - g_lastBlinktime) > BLINK_DURATION)
    {
        g_lastBlinktime = millis();
        digitalWrite(g_currentNode->pin, g_blinkLedState);
        g_blinkLedState = !g_blinkLedState;
    }
}

void moveToNextLed()
{
    int oX = analogRead(JOYSTICK_X);
    int oY = analogRead(JOYSTICK_Y);

    // return if no real joystick movement is detected
    if (oX > JOYSTICK_LOW_THRESHOLD && oX < JOYSTICK_HIGH_THRESHOLD && oY > JOYSTICK_LOW_THRESHOLD && oY < JOYSTICK_HIGH_THRESHOLD)
    {
        g_hasMoved = false;
        return;
    }

    if (g_hasMoved)
    {
        return;
    }

    // clean left
    if (oX < JOYSTICK_LOW_THRESHOLD && oY < JOYSTICK_HIGH_THRESHOLD && oY > JOYSTICK_LOW_THRESHOLD)
    {
        // for the moment leave the led closed
        digitalWrite(g_currentNode->pin, g_currentNode->state);
        g_lastBlinktime = BEGINNING_OF_APP_TIME;

        g_currentNode = g_currentNode->left;
        g_blinkLedState = !g_currentNode->state;
        g_hasMoved = true;
        return;
    }

    // clean right
    if (oX > JOYSTICK_HIGH_THRESHOLD && oY < JOYSTICK_HIGH_THRESHOLD && oY > JOYSTICK_LOW_THRESHOLD)
    {
        digitalWrite(g_currentNode->pin, g_currentNode->state);
        g_lastBlinktime = BEGINNING_OF_APP_TIME;

        g_currentNode = g_currentNode->right;
        g_blinkLedState = !g_currentNode->state;
        g_hasMoved = true;
        return;
    }

    // clean down
    if (oY > JOYSTICK_HIGH_THRESHOLD && oX < JOYSTICK_HIGH_THRESHOLD && oX > JOYSTICK_LOW_THRESHOLD)
    {
        digitalWrite(g_currentNode->pin, g_currentNode->state);
        g_lastBlinktime = BEGINNING_OF_APP_TIME;

        g_currentNode = g_currentNode->down;
        g_blinkLedState = !g_currentNode->state;
        g_hasMoved = true;
        return;
    }

    // clean up
    if (oY < JOYSTICK_LOW_THRESHOLD && oX < JOYSTICK_HIGH_THRESHOLD && oX > JOYSTICK_LOW_THRESHOLD)
    {
        digitalWrite(g_currentNode->pin, g_currentNode->state);
        g_lastBlinktime = BEGINNING_OF_APP_TIME;

        g_currentNode = g_currentNode->up;
        g_blinkLedState = !g_currentNode->state;
        g_hasMoved = true;
        return;
    }
}

void loop()
{
    flashCurrentLed();
    moveToNextLed();
}

void resetNodeStates()
{
    for(int nodeIdx = nodeIdxA; nodeIdx <= nodeIdxDP; nodeIdx++)
    {
        g_7segGraph[nodeIdx].state = LOW;
        digitalWrite(g_7segGraph[nodeIdx].pin, g_7segGraph[nodeIdx].state);
    }
}

void changeNodeState()
{
    if(millis() - g_lastPressTime < DURATION_OF_DEBOUNCE)
    {
        return;
    }

    if(g_fallingOrRising == FALLING)
    {
        Serial.print("falling\n");
    }
    else
    {
        Serial.print("rising\n");
    }
    if(millis() - g_lastPressTime > DURATION_FOR_RESET && g_fallingOrRising == FALLING)
    {
        resetNodeStates();
        Serial.print("am dat reset\n");
        g_fallingOrRising = !g_fallingOrRising;
        return;
    }

    if(g_fallingOrRising == RISING)
    {
        g_currentNode->state = !g_currentNode->state;
        Serial.print("changed State\n");
    }
    g_lastPressTime = millis();
    
    g_fallingOrRising = !g_fallingOrRising;

    Serial.print("\n");
}