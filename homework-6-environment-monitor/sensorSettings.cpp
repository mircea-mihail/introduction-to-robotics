#include "sensorSettings.h"

void printSensorSettings()
{
    Serial.print("\nSENSOR SETTINGS:\n");
    Serial.print("1 - Set sensors' sampling rate\n2 - set threshold value for ultrasonic sensor alert\n");
    Serial.print("3 - Set threshold value for brightness alert\n4 - Go back to main menu\n");
}

void pickSamplingInterval()
{

    Serial.print("Choose a value in ms between 1 and 10\n");

    // wait for input
    while(Serial.available() == NO_SERIAL_DATA);

    int inputSampleRate = Serial.parseInt();

    if(inputSampleRate >= MIN_SAMPLE_RATE && inputSampleRate <= MAX_SAMPLE_RATE)
    {
        g_sensorSamplingRate = inputSampleRate;
        Serial.print("updated the sampling value - ");
        Serial.println(g_sensorSamplingRate);
    }
    else
    {
        Serial.print(inputSampleRate);
        Serial.print(" - value not in range\n");
    }

    printSensorSettings();

}

void pickProximityThreshold()
{

    Serial.print("Pick a minimum allowed distance between 1 and 1023\n");

    // wait for input
    while(Serial.available() == NO_SERIAL_DATA);

    int proximityTreshold = Serial.parseInt();

    if(proximityTreshold >= MIN_ANALOG_IN && proximityTreshold <= MAX_ANALOG_IN)
    {
        g_proximityThresholdValue = proximityTreshold;
        Serial.print("updated the proximity value - ");
        Serial.println(g_proximityThresholdValue);
    }
    else
    {
        Serial.print(proximityTreshold);
        Serial.print(" - value not in range\n");
    }

    printSensorSettings();

}

void pickBrightnessThreshold()
{

    Serial.print("Pick a minimum allowed brightness between 1 and 1023\n");

    // wait for input
    while(Serial.available() == NO_SERIAL_DATA);

    int brightnessTreshold = Serial.parseInt();

    if(brightnessTreshold >= MIN_ANALOG_IN && brightnessTreshold <= MAX_ANALOG_IN)
    {
        g_brightnessThresholdValue = brightnessTreshold;
        Serial.print("updated the brightness value - ");
        Serial.println(g_brightnessThresholdValue);
    }
    else
    {
        Serial.print(brightnessTreshold);
        Serial.print(" - value not in range\n");
    }

    printSensorSettings();
}

void goToSensorSettings()
{ 
    if(g_sensorMenu == NOT_SELECTED)
    {
        printSensorSettings();
        g_sensorMenu = SELECTED;
    }

    if(Serial.available() > NO_SERIAL_DATA)
    {
        g_sensorMenu = Serial.parseInt();

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