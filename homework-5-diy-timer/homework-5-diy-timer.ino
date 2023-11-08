// shift register pins
#define LATCH_PIN 11  // Connects to STCP (latch pin) on the shift register
#define CLOCK_PIN 10  // Connects to SHCP (clock pin) on the shift register
#define DATA_PIN 12   // Connects to DS (data pin) on the shift register

#define D1_DISPLAY_PIN 4
#define D2_DISPLAY_PIN 5
#define D3_DISPLAY_PIN 6
#define D4_DISPLAY_PIN 7

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

void setup() 
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    //power off and init displays
    for (int i = 0; i < NUMBER_OF_DISPLAYS; i++) 
    {
        pinMode(displayDigits[i], OUTPUT);
        digitalWrite(displayDigits[i], HIGH);
    }

    Serial.begin(115200);
}

void loop() 
{
    if (millis() - g_lastIncrementMoment > DELAY_MILLIS) 
    {
        g_numberToDisplay++;
        
        // Wrap around after 9999
        g_numberToDisplay %= 10000;  
        g_lastIncrementMoment = millis();
    }
    displayNumber(g_numberToDisplay);
}

void writeReg(int digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(LATCH_PIN, LOW);

    // Shift out the byte representing the current digit to the shift register
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digit);

    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(LATCH_PIN, HIGH);
}

void activateDisplay(int displayNumber)
{
    // Turn off all digit control pins to avoid ghosting
    for (int i = 0; i < NUMBER_OF_DISPLAYS; i++)
    {
        digitalWrite(displayDigits[i], HIGH);
    }

    // Turn on the current digit control pin
    digitalWrite(displayDigits[displayNumber], LOW);
}

void displayNumber(int p_currentNumber) 
{
    int currentDisplay = 3;
    int digitToDisplay = 0;
    // loop through the displays
    while (p_currentNumber != 0) 
    {
        digitToDisplay = p_currentNumber % 10;
        activateDisplay(currentDisplay);

        writeReg(byteEncodings[digitToDisplay]);
        // delay(100);
        currentDisplay--;
        p_currentNumber /= 10;

        //prevent ghosting
        writeReg(B00000000);
    }
}
