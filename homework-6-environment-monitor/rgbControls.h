#ifndef RGB_CONTROLS_H
#define RGB_CONTROLS_H

#include "usefulDefines.h"
#include "Arduino.h"

#define MANUAL_RGB_CONTROL 1
#define TOGGLE_AUTOMATIC_RGB 2
#define GO_BACK 3

extern int g_lastBrightnessReading;
extern unsigned long g_lastBrightnessTime;
extern int g_lastBrightnessWriteAddr;

extern int g_lastProximityReading;
extern unsigned long g_lastProximityTime;
extern int g_lastProximityWriteAddr;

extern int g_proximityThresholdValue;
extern int g_brightnessThresholdValue;

extern byte g_rgbMenu;
extern byte g_mainMenu;

// main function that controls the automatic RGB values
void setRgbLight();

// rgb menu switch case
void goToRgbMenu();

#endif