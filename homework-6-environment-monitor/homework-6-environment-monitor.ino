#include "resetData.h"
#include "sensorSettings.h"
#include "usefulDefines.h"

// main menu
#define SENSOR_SETTINGS 1
#define RESET_DATA 2
#define SYSTEM_STATUS 3
#define CONTROL_RGB 4
#define COMING_BACK_TO_MAIN 12

// main menu vars
int g_mainMenu = SELECTED;
bool g_isOutsideTheMainMenu = false;

//reset data menu related vars
int g_resetDataMenu = NOT_SELECTED;

//sensor menu related vars
int g_sensorMenu = NOT_SELECTED;
int g_sensorSamplingRate = 1;
int g_proximityThresholdValue = 500;
int g_brightnessThresholdValue = 500;


void setup()
{
    Serial.begin(115200);
    printMainMenu();
}

void loop()
{
    if(Serial.available() > NO_SERIAL_DATA || g_mainMenu == COMING_BACK_TO_MAIN)
    {   
        if(g_mainMenu == SELECTED)
        {
            g_mainMenu = Serial.parseInt();
        }
        
        enterMainMenu();
    }
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

        // case SYSTEM_STATUS:
        //     break;

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