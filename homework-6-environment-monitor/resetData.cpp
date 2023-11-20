#include "resetData.h"

bool g_hasVisitedUltrasonic = false;
bool g_hasVisitedBrightness = false;

void printResetDataOptions()
{
    Serial.print(F("\nRESET DATA OPTIONS\n"));
    Serial.print(F("1 - Reset data for proximity sensor\n2 - Reset data for brightness sensor\n"));
    Serial.print(F("3 - Go back to main menu\n"));
}

bool areYouSure()
{
    int isSure = Serial.read();

    if(isSure == 'y')
    {
        return true;
    }
    return false;
}

void deleteUltrasonicData()
{
    if(!g_hasVisitedUltrasonic)
    {
        Serial.print(F("\nAre you sure you want to delete the data for the proximity sensor?\nType 'y' for yes, 'n' for no\n"));
        g_hasVisitedUltrasonic = true;
    }
    if(Serial.available() > NO_SERIAL_DATA)
    {
        if(areYouSure())
        {
            for (int i = ULTRASONIC_ADDRESS; i < ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE; i+= sizeof(int))
            {
                EEPROM.put(i, (int)RESET_VALUE);
            }
            Serial.print(F("Proximity sensor data has succesfully been deleted\n"));
        }
        else
        {
            Serial.print("No data has been erased\n");
        }
        printResetDataOptions();
        g_hasVisitedUltrasonic = false;
        g_resetDataMenu = SELECTED;
    }
}

void deleteBrightnessData()
{
    if(!g_hasVisitedBrightness)
    {
        Serial.print(F("\nAre you sure you want to delete the data for the brightness sensor?\nType 'y' for yes, 'n' for no\n"));
        g_hasVisitedBrightness = true;
    }
    if(Serial.available() > NO_SERIAL_DATA)
    {
        if(areYouSure())
        {
            for (int i = BRIGHTNESS_ADDRESS; i < BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE; i += sizeof(int))
            {
                EEPROM.put(i, (int)RESET_VALUE);
            }
            Serial.print(F("Brightness sensor data has succesfully been deleted\n"));
        }
        else
        {
            Serial.print(F("No data has been erased\n"));
        }
        printResetDataOptions();
        g_hasVisitedBrightness = false;
        g_resetDataMenu = SELECTED;
    }
}

void goToResetData()
{ 
    if(g_resetDataMenu == NOT_SELECTED)
    {
        printResetDataOptions();
        g_resetDataMenu = SELECTED;
    }

    if(Serial.available() > NO_SERIAL_DATA || g_resetDataMenu != SELECTED)
    {
        if(g_resetDataMenu == SELECTED)
        {
            g_resetDataMenu = Serial.parseInt();
        }

        switch (g_resetDataMenu)
        {
            case DELETE_ULTRASONIC_DATA:
                deleteUltrasonicData();
                break;
            
            case DELETE_BRIGHTNESS_DATA:
                deleteBrightnessData();
                break;
            
            case RETURN_FROM_DELETE_OPTIONS:
                g_mainMenu = COMING_BACK_TO_MAIN;
                g_resetDataMenu = NOT_SELECTED;
                break;

            default:
                g_resetDataMenu = SELECTED;
                printResetDataOptions();
                break;
        }
    }
}