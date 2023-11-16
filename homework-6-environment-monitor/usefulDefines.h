#ifndef USEFUL_DEFINES_H
#define USEFUL_DEFINES_H

// for all menus
#define NOT_SELECTED 10
#define SELECTED 11

// don't want to wear out the most used memory (0)
#define ULTRASONIC_ADDRESS 740
#define BRIGHTNESS_ADDRESS ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE // 10 logs and an average
#define COMING_BACK_TO_MAIN 12

// for input
#define NO_SERIAL_DATA 0
#define EEPROM_LOG_STORAGE 11

#endif