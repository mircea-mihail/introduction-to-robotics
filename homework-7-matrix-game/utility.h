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
#define IN_GAME 0
#define IN_MENU 1
#define IN_ANIMATION 2
#define LOST 3
#define WON 4

// player pins
#define JS_X_PIN A0
#define JS_Y_PIN A1
#define BUTTON_PIN 6

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

// reverses the direction. If it is up it makes it down. If it's right makes it left
#define reverseDirection(direction) (direction = (direction + (MAP_NUMBER_OF_ORIENTATIONS/2)) % MAP_NUMBER_OF_ORIENTATIONS) 

#define getColValFromByte(byteValue) ((byteValue >> col) & 1)

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