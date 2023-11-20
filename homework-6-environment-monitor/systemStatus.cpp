#include "systemStatus.h"

void printSystemStatus()
{
    Serial.print(F("\nSYSTEM STATUS: \n"));
    Serial.print(F("1 - Get current sensor readings (press q to quit this state)\n2 - Print sensor settings\n"));
    Serial.print(F("3 - Go back\n"));
}

void printSensorVariables()
{
    Serial.print(F("\nThe sampling rate is "));
    Serial.print(g_sensorSamplingRate / MILLIS_TO_SECONDS);
    Serial.print(" s\n");

    Serial.print(F("The proximity sensor threshold is "));
    Serial.print(g_proximityThresholdValue);
    Serial.print(F(" cm\n"));

    Serial.print(F("The brightness sensor threshold is "));
    Serial.print(g_brightnessThresholdValue);
    Serial.print(F(" out of 1023\n"));

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
            Serial.print(F("Stopping sensor readings...\n"));
            printSystemStatus();
            return;
        }
    }

    if(millis() - g_lastSensorReadTime >= g_sensorSamplingRate)
    {
        int logAverage;
        g_lastSensorReadTime = millis();
        
        Serial.print(F("Current Brightness: "));
        Serial.print(g_lastBrightnessReading);
        Serial.print(" and log average: ");
        EEPROM.get(BRIGHTNESS_AVERAGE_ADDRESS, logAverage);
        Serial.print(logAverage);
        Serial.println();

        Serial.print(F("Current Distance: "));
        Serial.print(g_lastProximityReading);
        Serial.print(" and log average: ");
        EEPROM.get(ULTRASONIC_AVERAGE_ADDRESS, logAverage);
        Serial.print(logAverage);

        Serial.println();
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