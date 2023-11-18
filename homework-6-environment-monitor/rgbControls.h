#ifndef RGB_CONTROLS_H
#define RGB_CONTROLS_H

#include "usefulDefines.h"
#include "Arduino.h"


extern int g_lastBrightnessReading;
extern unsigned long g_lastBrightnessTime;
extern int g_lastBrightnessWriteAddr;

extern int g_lastProximityReading;
extern unsigned long g_lastProximityTime;
extern int g_lastProximityWriteAddr;

extern int g_proximityThresholdValue;
extern int g_brightnessThresholdValue;

// main function that controls the automatic RGB values
void setRgbLight();

#endif