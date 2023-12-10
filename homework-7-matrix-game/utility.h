#ifndef UTILITY_H
#define UTILITY_H

// utility
#define MATRIX_ADDRESS 0
#define MATRIX_SIZE 16
#define REAL_MATRIX_SIZE 8
#define REAL_MATRIX_MIDDLE (REAL_MATRIX_SIZE / 2)
#define MATRIX_MIDDLE (MATRIX_SIZE / 2)

// generallys useful defines

// game states
#define GAME_IN_GAME 0
#define GAME_IN_MENU 1
#define GAME_IN_START_ANIMATION 2
#define GAME_LOST 3
#define GAME_WON 4
#define GAME_IN_MENU 5

// menu states
#define MENU_IN_START_GAME 0 
#define MENU_IN_SETTINGS 1
#define MENU_IN_ABOUT 2
// Not options but valid states
#define MENU_IN_GAME 3


// player pins
#define JS_X_PIN A0
#define JS_Y_PIN A1
#define BUTTON_PIN 13

// matrix occupation related
#define MAP_NUMBER_OF_ORIENTATIONS 4
#define MAP_BULLET_RANGE 1

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_BULLET 3
#define MAP_POWER_UP 4

// facing direction of player/bullet/other things
#define DIRECTION_UP 0
#define DIRECTION_LEFT 1 
#define DIRECTION_DOWN 2
#define DIRECTION_RIGHT 3

// bullet defines
#define BULLET_TICK_RATE 200
#define SHOOTING_COOLDOWN (BULLET_TICK_RATE)
#define DEFAULT_RANGE 2
#define BULLET_BLINK_INTERVAL 30

// player defines 
#define PLAYER_MOVEMENT_MILLIS 400
#define PLAYER_BLINK_INTERVAL 200

// power up defines
#define POWER_UP_BLINK_INTERVAL 500

// animation defines
#define DEFAULT_TIME_VAL 0

// sensor defines
#define MAX_BRIGHTNESS_INPUT 801
#define MIN_BRIGHTNESS_INPUT 800

#define MAX_MATRIX_BRIGHTNESS 15
#define MIN_MATRIX_BRIGHTNESS 0

// joystick 
#define JS_DEFAULT_VALUE 512
#define MIN_JS_THRESHOLD 312
#define MAX_JS_THRESHOLD 712

// LCD display
#define RESET 9
#define ENABLE 8
#define DATA4 7
#define DATA5 6
#define DATA6 5
#define DATA7 4
#define LCD_CONTRAST 3

// icons for the menu
#define NUMBER_OF_ICONS 1
#define ICON_HAMMER 0

// misc peripherals
#define BRIGHTNESS_PIN A2

// reverses the direction. If it is up it makes it down. If it's right makes it left
#define reverseDirection(direction) (direction = (direction + (MAP_NUMBER_OF_ORIENTATIONS/2)) % MAP_NUMBER_OF_ORIENTATIONS) 

// when a column in a matrix is encoded in a byte this functions returns the saught after bit value
#define getColValFromByte(byteValue, col) ((byteValue >> col) & 1)

// returns the absoulte value of a number
inline int absolute(int a) 
{
    if(a < 0)
    {
        return a * -1;
    }
    else 
    {
        return a;
    }
}

#endif