// shift register pins
#define LATCH_PIN 11  // Connects to STCP (latch pin) on the shift register
#define CLOCK_PIN 10  // Connects to SHCP (clock pin) on the shift register
#define DATA_PIN 12   // Connects to DS (data pin) on the shift register

#define D1_DISPLAY_PIN 4
#define D2_DISPLAY_PIN 5
#define D3_DISPLAY_PIN 6
#define D4_DISPLAY_PIN 7

#define START_STOP_PIN 3
#define RESET_PIN 9
#define LAP_PIN 2

#define DP_BYTE_MASK B00000001
// Store the digits in an array for easy access
int displayDigits[] = { D1_DISPLAY_PIN, D2_DISPLAY_PIN, D3_DISPLAY_PIN, D4_DISPLAY_PIN };

#define NUMBER_OF_DISPLAYS 4

#define NUMBER_OF_ENCODINGS 16
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[NUMBER_OF_ENCODINGS] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};

#define DELAY_MILLIS 100
unsigned long g_lastIncrementMoment = 0;
unsigned long g_numberToDisplay = 0;       // The number being displayed

//if current display is 2, also display a dot
#define getByteForDisplay(currentDisplay, digitToDisplay) ((currentDisplay == 2) ? \
                                                            (byteEncodings[digitToDisplay] | DP_BYTE_MASK) : \
                                                            (byteEncodings[digitToDisplay]))
// for button presses:
#define MILLIS_TO_MICROS 1000U
#define DEBOUNCE_MICROS (50UL * MILLIS_TO_MICROS)
#define DURATION_FOR_CYCLE (500UL * MILLIS_TO_MICROS)

unsigned long g_lastPressTime = 0;
bool g_timerIsRunning = false;
unsigned short g_fallingOrRising = RISING;


void setup() 
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    pinMode(START_STOP_PIN, INPUT_PULLUP);
    pinMode(RESET_PIN, INPUT_PULLUP);
    pinMode(LAP_PIN, INPUT_PULLUP);

    //power off and init displays
    for (int i = 0; i < NUMBER_OF_DISPLAYS; i++) 
    {
        pinMode(displayDigits[i], OUTPUT);
        digitalWrite(displayDigits[i], HIGH);
    }
    attachInterrupt(digitalPinToInterrupt(START_STOP_PIN), startStopDebounce, RISING);
    attachInterrupt(digitalPinToInterrupt(LAP_PIN), startStopDebounce, CHANGE);

    Serial.begin(115200);
}

void loop() 
{
    if (millis() - g_lastIncrementMoment > DELAY_MILLIS && g_timerIsRunning) 
    {
        g_numberToDisplay++;
        
        // Wrap around after 9999
        g_numberToDisplay %= 10000;  
        g_lastIncrementMoment = millis();
    }
    displayNumber(g_numberToDisplay);
    resetIfNecessary();
}

void resetIfNecessary()
{
    
}

void writeReg(int digit) 
{
    digitalWrite(LATCH_PIN, LOW);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digit);

    digitalWrite(LATCH_PIN, HIGH);
}

void activateDisplay(int displayNumber)
{
    for (int i = 0; i < NUMBER_OF_DISPLAYS; i++)
    {
        digitalWrite(displayDigits[i], HIGH);
    }

    digitalWrite(displayDigits[displayNumber], LOW);
}

void displayNumber(int p_currentNumber) 
{
    int currentDisplay = 3;
    int digitToDisplay = 0;
    byte byteToWrite;
    // loop through the displays
    while (currentDisplay >= 0) 
    {
        digitToDisplay = p_currentNumber % 10;
        activateDisplay(currentDisplay);

        byteToWrite = getByteForDisplay(currentDisplay, digitToDisplay);
        
        writeReg(byteToWrite);
        // delay(100);
        currentDisplay--;
        p_currentNumber /= 10;

        //prevent ghosting
        writeReg(B00000000);
    }
}

void startStopDebounce()
{
    if(micros() - g_lastPressTime < DEBOUNCE_MICROS)
    {
        return;
    }

    if(!g_timerIsRunning)
    {
        Serial.print("started the timer\n");
        g_timerIsRunning = true;
    }
    else if(g_timerIsRunning)
    {
        Serial.print("stopped the timer\n");
        g_timerIsRunning = false;
    }

    g_lastPressTime = micros();    
}


void lapDebounce()
{
    if(micros() - g_lastPressTime < DEBOUNCE_MICROS)
    {
        return;
    }

    // if(micros() - g_lastPressTime > DURATION_FOR_RESET)
    // {
    //     //enable variable to cycle through 
    // }
    if(g_fallingOrRising == FALLING)
    {
        //disable cycle possibility
    }

    if(g_fallingOrRising == RISING)
    {
        //start a timer to enable cycle through laps 

        if(!g_timerIsRunning)
        {
            // cycle through lap
        }
        else if(g_timerIsRunning)
        {
            //save lap round robin
        }
    }

    g_lastPressTime = micros();    

    //if falling make rising and reverse 
    g_fallingOrRising = (g_fallingOrRising == RISING ? FALLING : RISING);
}