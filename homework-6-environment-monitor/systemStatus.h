#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H

#include "usefulDefines.h"
#include "Arduino.h"

#define GET_SENSOR_READINGS 1
#define PRINT_SENSOR_SETTINGS 2
#define GO_BACK 3

extern byte g_systemStatus;
extern byte g_mainMenu;

extern int g_proximityThresholdValue;
extern int g_brightnessThresholdValue;
extern int g_sensorSamplingRate;

extern int g_lastBrightnessReading;
extern unsigned long g_lastBrightnessTime;
extern int g_lastBrightnessWriteAddr;

extern int g_lastProximityReading;
extern unsigned long g_lastProximityTime;
extern int g_lastProximityWriteAddr;

extern unsigned long g_lastSensorReadTime;

// the main switch case submenu of the system status
void goToSystemStatus();

// enables the option to print sensor readings continuously until the user presses q
void printSensorReadings();

// prints to the serial the sensor variables such as sampling rate and thresholds
void printSensorVariables();

// prints the menu options coded withing the switch
void printSystemStatus();


#endif