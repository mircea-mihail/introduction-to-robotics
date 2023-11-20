#include "sensorSettings.h"

bool g_hasVisitedSamplingInterval = false;
bool g_hasVisitedProximityThreshold = false;
bool g_hasVisitedBrightnessThreshold = false;

void printSensorSettings()
{
    Serial.print(F("\nSENSOR SETTINGS:\n"));
    Serial.print(F("1 - Set sensors' sampling rate\n2 - set threshold value for ultrasonic sensor alert\n"));
    Serial.print(F("3 - Set threshold value for brightness alert\n4 - Go back to main menu\n"));
}

void pickSamplingInterval()
{
    if(!g_hasVisitedSamplingInterval)
    {
        Serial.print(F("Choose a value in ms between 1 and 10\n"));
        g_hasVisitedSamplingInterval = true;
    }

    // wait for input
    if(Serial.available() > NO_SERIAL_DATA)
    {
        int inputSampleRate = Serial.parseInt();

        if(inputSampleRate >= MIN_SAMPLE_RATE && inputSampleRate <= MAX_SAMPLE_RATE)
        {
            g_sensorSamplingRate = inputSampleRate * MILLIS_TO_SECONDS;
            Serial.print(F("updated the sampling value - "));
            Serial.println(g_sensorSamplingRate / MILLIS_TO_SECONDS);
        }
        else
        {
            Serial.print(inputSampleRate);
            Serial.print(F(" - value not in range\n"));
        }

        printSensorSettings();
        g_hasVisitedSamplingInterval = false;
        g_sensorMenu = SELECTED;
    }
}

void pickProximityThreshold()
{
    if(!g_hasVisitedProximityThreshold)
    {
        Serial.print(F("Pick a minimum allowed distance between 1 and 1023\n"));
        g_hasVisitedProximityThreshold = true;
    }

    if(Serial.available() > NO_SERIAL_DATA)
    {
        int proximityTreshold = Serial.parseInt();

        if(proximityTreshold >= MIN_ANALOG_IN && proximityTreshold <= MAX_ANALOG_IN)
        {
            g_proximityThresholdValue = proximityTreshold;
            Serial.print(F("updated the proximity value - "));
            Serial.println(g_proximityThresholdValue);
        }
        else
        {
            Serial.print(proximityTreshold);
            Serial.print(F(" - value not in range\n"));
        }

        printSensorSettings();
        g_hasVisitedProximityThreshold = false;
        g_sensorMenu = SELECTED;
    }
}

void pickBrightnessThreshold()
{
    if(!g_hasVisitedBrightnessThreshold)
    {
        Serial.print(F("Pick a minimum allowed brightness between 1 and 1023\n"));
        g_hasVisitedBrightnessThreshold = true;
    }

    if(Serial.available() > NO_SERIAL_DATA)
    {
        int brightnessTreshold = Serial.parseInt();

        if(brightnessTreshold >= MIN_ANALOG_IN && brightnessTreshold <= MAX_ANALOG_IN)
        {
            g_brightnessThresholdValue = brightnessTreshold;
            Serial.print(F("updated the brightness value - "));
            Serial.println(g_brightnessThresholdValue);
        }
        else
        {
            Serial.print(brightnessTreshold);
            Serial.print(F(" - value not in range\n"));
        }

        printSensorSettings();
        g_hasVisitedBrightnessThreshold = false;
        g_sensorMenu = SELECTED;
    }
}

void goToSensorSettings()
{ 
    if(g_sensorMenu == NOT_SELECTED)
    {
        printSensorSettings();
        g_sensorMenu = SELECTED;
    }

    if(Serial.available() > NO_SERIAL_DATA || g_sensorMenu != SELECTED)
    {
        if(g_sensorMenu == SELECTED)
        {
            g_sensorMenu = Serial.parseInt();
        }

        switch (g_sensorMenu)
        {
            case SAMPLING_INTERVAL:
                pickSamplingInterval();
                break;
            
            case PROXIMITY_THRESHOLD:
                pickProximityThreshold();
                break;
            
            case BRIGHTNESS_THRESHOLD:
                pickBrightnessThreshold();
                break;
            
            case RETURN_FROM_SENSOR_SETTINGS:
                g_mainMenu = COMING_BACK_TO_MAIN;
                g_sensorMenu = NOT_SELECTED;
                break;

            default:
                g_sensorMenu = SELECTED;
                printSensorSettings();
                break;
        }
    }
}