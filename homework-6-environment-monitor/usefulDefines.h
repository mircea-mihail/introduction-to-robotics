#ifndef USEFUL_DEFINES_H
#define USEFUL_DEFINES_H

// for all menus
#define NOT_SELECTED 10
#define SELECTED 11

// don't want to wear out the most used memory (0)
#define STORED_DATA_SIZE 10
#define ULTRASONIC_ADDRESS 740
#define EEPROM_LOG_STORAGE (11 * sizeof(int))
#define BRIGHTNESS_ADDRESS (ULTRASONIC_ADDRESS + EEPROM_LOG_STORAGE)// 10 logs and an average

#define COMING_BACK_TO_MAIN 12

// for input
#define NO_SERIAL_DATA 0

// sensor related
#define MILLIS_TO_SECONDS 1000UL
#define ULTRASONIC_TRIG 11
#define ULTRASONIC_ECHO 10

//rgb
#define RED_LED_PIN 6
#define GREEN_LED_PIN 5
#define BLUE_LED_PIN 3

#define LED_ON 30
#define LED_OFF 0

#endif