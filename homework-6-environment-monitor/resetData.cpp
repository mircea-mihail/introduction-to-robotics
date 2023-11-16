#include "resetData.h"

void printResetDataOptions()
{
    Serial.print("\nRESET DATA OPTIONS\n");
    Serial.print("1 - Reset data for proximity sensor\n2 - Reset data for brightness sensor\n");
    Serial.print("3 - Go back to main menu\n");
}

bool areYouSure()
{
    while(Serial.available() == NO_SERIAL_DATA);
    int isSure = Serial.read();

    if(isSure == 'y')
    {
        return true;
    }
    return false;
}

void deleteUltrasonicData()
{
    Serial.print("\nAre you sure you want to delete the data for the proximity sensor?\nType 'y' for yes, 'n' for no\n");
    if(areYouSure())
    {
        for (int i = ULTRASONIC_ADDRESS; i < ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE; i++)
        {
            EEPROM.update(i, 0);
        }
        Serial.print("Proximity sensor data has succesfully been deleted\n");
    }
    else
    {
        Serial.print("No data has been erased\n");
    }
    printResetDataOptions();
}

void deleteBrightnessData()
{
    Serial.print("\nAre you sure you want to delete the data for the brightness sensor?\nType 'y' for yes, 'n' for no\n");
    if(areYouSure())
    {
        for (int i = BRIGHTNESS_ADDRESS; i < BRIGHTNESS_ADDRESS + EEPROM_LOG_STORAGE; i++)
        {
            EEPROM.update(i, 0);
        }
        Serial.print("Brightness sensor data has succesfully been deleted\n");
    }
    else
    {
        Serial.print("No data has been erased\n");
    }
    printResetDataOptions();
}

void goToResetData()
{ 
    if(g_resetDataMenu == NOT_SELECTED)
    {
        printResetDataOptions();
        g_resetDataMenu = SELECTED;
    }

    if(Serial.available() > NO_SERIAL_DATA)
    {
        g_resetDataMenu = Serial.parseInt();

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