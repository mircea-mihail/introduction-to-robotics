//todo
// test memory erase
// change threshold values ( make them in cm and lumen or smth )

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

// reset data menu related vars
int g_resetDataMenu = NOT_SELECTED;

// sensor menu related vars
int g_sensorMenu = NOT_SELECTED;
int g_sensorSamplingRate = 1;
int g_proximityThresholdValue = 500;
int g_brightnessThresholdValue = 500;

// system status
int g_systemStatus = NOT_SELECTED;


void setup()
{
    Serial.begin(115200);
    printMainMenu();
}

void loop()
{
    if(Serial.available() > NO_SERIAL_DATA || g_mainMenu != SELECTED)
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

#define GET_SENSOR_READINGS 1
#define PRINT_SENSOR_SETTINGS 2
#define GO_BACK 3


void printSystemStatus()
{
    Serial.print("SYSTEM STATUS: \n");
    Serial.print("1 - Get current sensor readings (press q to quit this state)\n2 - Print sensor settings\n");
    Serial.print("3 - Go back\n");
}

void printSensorVariables()
{
    Serial.print("\nThe sampling rate is ");
    Serial.print(g_sensorSamplingRate);
    Serial.print(" s\n");

    Serial.print("The proximity sensor threshold is ");
    Serial.print(g_proximityThresholdValue);
    Serial.print(" <unit>\n");

    Serial.print("The brightness sensor threshold is ");
    Serial.print(g_brightnessThresholdValue);
    Serial.print(" <unit>\n");

    printSystemStatus();
}

void printSensorReadings()
{
    if(Serial.available() != NO_SERIAL_DATA)
    {
        int userInput = Serial.read();
        if(userInput == 'q')
        {
            // stops the bypass of system status
            g_systemStatus = SELECTED;
            Serial.print("Stopping sensor readings...\n");
            printSystemStatus();
            return;
        }
    }

    Serial.print("it works!\n");
    delay(1000);
}

void goToSystemStatus()
{
    if(g_systemStatus == NOT_SELECTED)
    {
        printSystemStatus();
        g_systemStatus = SELECTED;
    }

    // bypass serail available to constantly print sensor readings
    if(g_systemStatus == GET_SENSOR_READINGS || Serial.available() > NO_SERIAL_DATA)
    {
        // if we want to constantly print sensor readings only from the sensor readings can the system status be changed
        if(g_systemStatus != GET_SENSOR_READINGS)
        {
            g_systemStatus = Serial.parseInt();
        }

        switch (g_systemStatus)
        {
            case GET_SENSOR_READINGS:
                printSensorReadings();
                break;
            
            case PRINT_SENSOR_SETTINGS:
                printSensorVariables();
                break;

            case GO_BACK:
                g_mainMenu = COMING_BACK_TO_MAIN;
                g_systemStatus = NOT_SELECTED;
                break;

            default:
                g_systemStatus = SELECTED;
                printSystemStatus();
                break;
        }
    }
}
