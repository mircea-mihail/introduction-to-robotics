#ifndef RESET_DATA_H
#define RESET_DATA_H

#include <EEPROM.h>
#include <Arduino.h>

#include "usefulDefines.h"

#define DELETE_ULTRASONIC_DATA 1
#define DELETE_BRIGHTNESS_DATA 2
#define RETURN_FROM_DELETE_OPTIONS 3

#define RESET_VALUE 0

extern int g_resetDataMenu;
extern int g_mainMenu;

// just prints the reset data menu
extern void printResetDataOptions();

// waits for input and returns true if input was 'y'
extern bool areYouSure();

// erases the memory location where the ultrasonic data is (sets it to 0) 
extern void deleteUltrasonicData();

// erases the memory location where the brightness data is (sets it to 0)
extern void deleteBrightnessData();

// the switch case code that contains the menu logic 
extern void goToResetData();

#endif // RESET_DATA_H