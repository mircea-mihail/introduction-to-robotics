//todo
// change threshold prompt to fit 
// different sample rate for each sensor?

#include "resetData.h"
#include "sensorSettings.h"
#include "usefulDefines.h"
#include "systemStatus.h"
#include "rgbControls.h"

// main menu
#define SENSOR_SETTINGS 1
#define RESET_DATA 2
#define SYSTEM_STATUS 3
#define CONTROL_RGB 4
#define COMING_BACK_TO_MAIN 12

// sensor defines
#define LIGHT_DETECTOR_PIN A0
#define ULTRASONIC_TRIG_WAIT_US 2
#define ULTRASONIC_HIGH_TRIG_DURATION_US 2
#define SPEED_OF_LIGHT_CM_PER_US 0.0343
#define ULTRASONIC_TRAVELS_TWO_WAYS 2

// main menu vars
byte g_mainMenu = SELECTED;
bool g_isOutsideTheMainMenu = false;

// reset data menu related vars
byte g_resetDataMenu = NOT_SELECTED;

// sensor menu related vars
byte g_sensorMenu = NOT_SELECTED;
int g_sensorSamplingRate = 10 * MILLIS_TO_SECONDS;
int g_proximityThresholdValue = 5;
int g_brightnessThresholdValue = 500;

// system status
byte g_systemStatus = NOT_SELECTED;

// sensor values
int g_lastBrightnessReading = 0;
unsigned long g_lastBrightnessTime = 0;
int g_lastBrightnessWriteAddr = BRIGHTNESS_ADDRESS;

int g_lastProximityReading = 0;
unsigned long g_lastProximityTime = 0;
int g_lastProximityWriteAddr = ULTRASONIC_ADDRESS;

unsigned long g_lastSensorReadTime = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LIGHT_DETECTOR_PIN, INPUT);

    pinMode(ULTRASONIC_ECHO, INPUT);
    pinMode(ULTRASONIC_TRIG, OUTPUT);
    
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    printMainMenu();
}

void checkMemory()
{
    // check memory
    Serial.print("brightness values: \n");
    for(int i = BRIGHTNESS_ADDRESS ; i < BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE; i += sizeof(int))
    {
        int brightnessValue;
        EEPROM.get(i, brightnessValue);
        Serial.print(brightnessValue);
        Serial.print(' ');
    }

    Serial.print("\n\nproximity values: \n");
    for(int i = ULTRASONIC_ADDRESS ; i < ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE; i += sizeof(int))
    {
        int ultrasonicValue;
        EEPROM.get(i, ultrasonicValue);
        Serial.print(ultrasonicValue);
        Serial.print(' ');
    }
    Serial.print("\n");
}

void readSensorValues()
{
    // brightness sensor
    if(millis() - g_lastBrightnessTime > g_sensorSamplingRate)
    {
        g_lastBrightnessReading = analogRead(LIGHT_DETECTOR_PIN);
        // Serial.print(g_lastBrightnessReading);
        // Serial.print("\n");
        g_lastBrightnessTime = millis();
        
        //write to memory
        EEPROM.put(g_lastBrightnessWriteAddr, g_lastBrightnessReading);
        g_lastBrightnessWriteAddr += sizeof(int);

        // last int is for an average
        if(g_lastBrightnessWriteAddr >= BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int))
        {
            g_lastBrightnessWriteAddr = BRIGHTNESS_ADDRESS;
        }

        // do average
        int brightnessAverage = 0;
        for(int i = BRIGHTNESS_ADDRESS ; i < BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int); i += sizeof(int))
        {
            int brightnessValue;
            EEPROM.get(i, brightnessValue);
            brightnessAverage += brightnessValue / STORED_DATA_SIZE;
        }
        EEPROM.put(BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int), brightnessAverage);
    }

    //  ultrasonic sensor
    if(millis() - g_lastProximityTime > g_sensorSamplingRate)
    {
        digitalWrite(ULTRASONIC_TRIG, LOW);
        delayMicroseconds(ULTRASONIC_TRIG_WAIT_US);

        digitalWrite(ULTRASONIC_TRIG, HIGH);
        delayMicroseconds(ULTRASONIC_HIGH_TRIG_DURATION_US);
        digitalWrite(ULTRASONIC_TRIG, LOW);

        long travelDuration = pulseIn(ULTRASONIC_ECHO, HIGH);
        g_lastProximityReading = travelDuration * SPEED_OF_LIGHT_CM_PER_US/ULTRASONIC_TRAVELS_TWO_WAYS;

        g_lastProximityTime = millis();

        // write to memory
        EEPROM.put(g_lastProximityWriteAddr, g_lastProximityReading);
        g_lastProximityWriteAddr += sizeof(int);

        // last int is for an average
        if(g_lastProximityWriteAddr >= ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int))
        {
            g_lastProximityWriteAddr = ULTRASONIC_ADDRESS;
        }

        // do average
        int ultrasonicAverage = 0;
        for(int i = ULTRASONIC_ADDRESS ; i < ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int); i += sizeof(int))
        {
            int ultrasonicValue;
            EEPROM.get(i, ultrasonicValue);
            ultrasonicAverage += ultrasonicValue / STORED_DATA_SIZE;
        }
        EEPROM.put(ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE - sizeof(int), ultrasonicAverage);
    }
}


unsigned long checkMemoryTime = 0;

void loop()
{
    readSensorValues();

    setRgbLight();

    if(Serial.available() > NO_SERIAL_DATA || g_mainMenu != SELECTED)
    {   
        if(g_mainMenu == SELECTED)
        {
            g_mainMenu = Serial.parseInt();
        }
        
        enterMainMenu();
    }

    // if(millis() - checkMemoryTime > 10000)
    // {
    //     checkMemoryTime = millis();
    //     checkMemory();
    // }
}

void enterMainMenu()
{
    switch(g_mainMenu)
    {
        case SENSOR_SETTINGS:
            goToSensorSettings();
            break;

        case RESET_DATA:
            goToResetData();
            break;

        case SYSTEM_STATUS:
            goToSystemStatus();
            break;

        // case CONTROL_RGB:
        //     break;

        case COMING_BACK_TO_MAIN:
            printMainMenu();
            g_mainMenu = SELECTED;
            break;

        default:
            //bad user input: show him his options
            g_mainMenu = SELECTED;
            printMainMenu();
            break;
    }
}

void printMainMenu()
{
    Serial.print("\nMAIN MENU: \n");
    Serial.print("1 - Tweak sensor settings\n2 - Reset logged data\n");
    Serial.print("3 - Check system status\n4 - Control the RGB LED\n");
}