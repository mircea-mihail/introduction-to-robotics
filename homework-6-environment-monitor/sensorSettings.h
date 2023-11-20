#ifndef SENSOR_SETTINGS_H
#define SENSOR_SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "usefulDefines.h"

#define MIN_ANALOG_IN 0
#define MAX_ANALOG_IN 1023

// sensor settings options
#define SAMPLING_INTERVAL 1
#define PROXIMITY_THRESHOLD 2
#define BRIGHTNESS_THRESHOLD 3
#define RETURN_FROM_SENSOR_SETTINGS 4

extern byte g_mainMenu;
extern byte g_sensorMenu;

//for sensors
extern int g_sensorSamplingRate;
extern int g_proximityThresholdValue;
extern int g_brightnessThresholdValue;

// prints the menu for the sensor settings
void printSensorSettings();

// allows the user to pick the sampling interval and sets it
void pickSamplingInterval();

// allows the user to pick the proximity threshold and sets it
void pickProximityThreshold();

// allows the user to pick the brightness threshold and sets it
void pickBrightnessThreshold();

// the main sensor settings menu with the switch case
 void goToSensorSettings();

#endif