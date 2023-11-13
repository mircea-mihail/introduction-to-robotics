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
volatile unsigned long g_counterInMs = 0;       
volatile unsigned long g_numberToDisplay = 0;   
volatile unsigned long g_numberAtLastLap = 0;

#define NUMBER_OF_LAPS_SAVED 4
volatile short g_lapIterator = 0;
volatile unsigned long g_lapsSaved[4] = {0, 0, 0, 0};

//if current display is 2, also display a dot
#define getByteForDisplay(currentDisplay, digitToDisplay) ((currentDisplay == 2) ? \
                                                            (byteEncodings[digitToDisplay] | DP_BYTE_MASK) : \
                                                            (byteEncodings[digitToDisplay]))
// for button presses:
#define MILLIS_TO_MICROS 1000U
#define DEBOUNCE_MICROS (40UL * MILLIS_TO_MICROS)
#define DURATION_FOR_CYCLE (500UL * MILLIS_TO_MICROS)

volatile unsigned long g_lastPressTime = 0;
volatile bool g_timerIsRunning = false;
volatile unsigned short g_fallingOrRising = RISING;

//for cycling on hold
#define TIME_TO_HOLD_FOR_CYCLE 500UL * MILLIS_TO_MICROS
volatile unsigned long g_lastCycleTime = 0;
volatile bool g_shouldCycle = false;

//for reset button debounce
byte g_resetBtnState = LOW;
byte g_lastCountedResetBtnState = LOW;
unsigned long g_previousResetBtnTime = 0;

//for lapping
#define getPreviousIterator() (g_lapIterator != 0 ? (g_lapIterator - 1) : (NUMBER_OF_LAPS_SAVED - 1))
#define getNextIterator() ((g_lapIterator + 1) % NUMBER_OF_LAPS_SAVED)

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
    attachInterrupt(digitalPinToInterrupt(LAP_PIN), lapDebounce, CHANGE);

    Serial.begin(115200);
}

void loop() 
{
    if(g_timerIsRunning)
    {
        g_shouldCycle = false;
    
        if (millis() - g_lastIncrementMoment > DELAY_MILLIS) 
        {
            g_counterInMs++;
            
            // Wrap around after 9999
            g_counterInMs %= 10000;  
            g_lastIncrementMoment = millis();
        }

        displayNumber(g_counterInMs);
    }
    else
    if(g_timerIsRunning == 0)
    {
        //can only reset when timer not running
        resetIfNecessary();
        
        displayNumber(g_numberToDisplay);
    
        cycleIfNecessary();
    }
}


////////////////////////////////////////////// GENERAL UTILITY /////////////////////

void cycleIfNecessary()
{
    if(g_shouldCycle)
    {
        if(micros() - g_lastCycleTime > TIME_TO_HOLD_FOR_CYCLE)
        {
            cycleThroughLaps();
            g_lastCycleTime = micros();
        }
    }
}

// resets laps or counter if appropriate
void resetIfNecessary()
{
    if(resetDebounce())
    {
        if(g_counterInMs != 0)
        {
            g_counterInMs = 0;
            g_numberAtLastLap = 0;
            g_numberToDisplay = g_counterInMs;
        }
        // don't reset laps if they have not even been viewed
        else if(g_numberToDisplay != 0)
        {
            for(int lapsLeft = 0; lapsLeft < NUMBER_OF_LAPS_SAVED; lapsLeft++)
            {
                g_lapsSaved[g_lapIterator] = 0;
                g_lapIterator = getNextIterator();
            }
            g_numberToDisplay = g_lapsSaved[g_lapIterator];

        }
    }
}

// cycle through laps but also show the counter
// don't show empty laps unless there are only empty laps    
void cycleThroughLaps()
{
    if(g_lapsSaved[g_lapIterator] != 0)
    {
        g_numberToDisplay = g_lapsSaved[g_lapIterator];
        g_lapIterator = getNextIterator();
    }
    else
    {
        for(int i = 0; i < NUMBER_OF_LAPS_SAVED; i++)
        {
            if(g_lapsSaved[g_lapIterator] != 0)
            {
                g_numberToDisplay = g_lapsSaved[g_lapIterator];
                g_lapIterator = getNextIterator();
                break;
            }
            g_lapIterator = getNextIterator();

        }
    }
}


////////////////////////////////////////////// DISPLAY RELATED /////////////////////

//display any number on the LCD
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

//write a register to shifter
void writeReg(int digit) 
{
    digitalWrite(LATCH_PIN, LOW);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digit);

    digitalWrite(LATCH_PIN, HIGH);
}

//only activate one display at a time at the 7 seg 4 display module
void activateDisplay(int displayNumber)
{
    for (int i = 0; i < NUMBER_OF_DISPLAYS; i++)
    {
        digitalWrite(displayDigits[i], HIGH);
    }

    digitalWrite(displayDigits[displayNumber], LOW);
}


////////////////////////////////////////////// BUTTONS /////////////////////

//returns true if a valid reset button press happened
bool resetDebounce()
{
	unsigned long time = millis();
    byte previousState = g_resetBtnState;
	g_resetBtnState = !digitalRead(RESET_PIN);

	// if the button has a constant state
	if(g_resetBtnState == previousState)
	{
		// if the constant state has been kept for a while
		if(time - g_previousResetBtnTime > (DEBOUNCE_MICROS / MILLIS_TO_MICROS) && g_lastCountedResetBtnState != g_resetBtnState)
		{
			g_lastCountedResetBtnState = g_resetBtnState;

			if(g_resetBtnState == HIGH)
			{
				previousState = g_resetBtnState;
				return true;
			}
		}
	}
	else
	{
		g_previousResetBtnTime = time;
	}

	return false;
}

// know when to start and stop the counter 
void startStopDebounce()
{
    if(micros() - g_lastPressTime < DEBOUNCE_MICROS)
    {
        return;
    }

    if(!g_timerIsRunning)
    {
        g_timerIsRunning = true;
    }
    else if(g_timerIsRunning)
    {
        g_timerIsRunning = false;
        g_numberToDisplay = g_counterInMs;
    }

    g_lastPressTime = micros();    
}

// deal with all the lap stuff -> save, cycle and hold cycle
void lapDebounce()
{
    if(micros() - g_lastPressTime < DEBOUNCE_MICROS)
    {
        return;
    }
    g_lastPressTime = micros();    

    // if(micros() - g_lastPressTime > DURATION_FOR_RESET)
    // {
    //     //enable variable to cycle through 
    // }
    if(g_fallingOrRising == FALLING)
    {
        g_shouldCycle = false;
        //disable cycle possibility
    }

    if(g_fallingOrRising == RISING)
    {
        g_shouldCycle = true;
        g_lastCycleTime = micros();

        if(!g_timerIsRunning && g_counterInMs == 0)
        {
            cycleThroughLaps();
        }
        else if(g_timerIsRunning)
        {
            // store laps
            g_lapsSaved[g_lapIterator] = g_counterInMs - g_numberAtLastLap;
            g_numberAtLastLap = g_counterInMs;
            g_lapIterator = getNextIterator();
        }
    }
    
    //if falling make rising and reverse 
    g_fallingOrRising = (g_fallingOrRising == RISING ? FALLING : RISING);
}
