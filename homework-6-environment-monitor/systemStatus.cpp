#include "systemStatus.h"

void printSystemStatus()
{
    Serial.print("\nSYSTEM STATUS: \n");
    Serial.print("1 - Get current sensor readings (press q to quit this state)\n2 - Print sensor settings\n");
    Serial.print("3 - Go back\n");
}

void printSensorVariables()
{
    Serial.print("\nThe sampling rate is ");
    Serial.print(g_sensorSamplingRate / MILLIS_TO_SECONDS);
    Serial.print(" s\n");

    Serial.print("The proximity sensor threshold is ");
    Serial.print(g_proximityThresholdValue);
    Serial.print(" cm\n");

    Serial.print("The brightness sensor threshold is ");
    Serial.print(g_brightnessThresholdValue);
    Serial.print(" out of 1023\n");

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

    if(millis() - g_lastSensorReadTime >= g_sensorSamplingRate)
    {
        g_lastSensorReadTime = millis();
        
        Serial.print("Current Brightness: ");
        Serial.println(g_lastBrightnessReading);

        Serial.print("Current Distance: ");
        Serial.println(g_lastProximityReading);
        Serial.println();
    }
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