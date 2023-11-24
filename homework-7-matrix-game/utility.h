#ifndef UTILITY_H
#define UTILITY_H

// utility
#define MATRIX_ADDRESS 0
#define MATRIX_SIZE 8
#define MATRIX_MIDDLE (MATRIX_SIZE / 2)

// generallys useful defines

// matrix occupation related
#define MAP_NUMBER_OF_ORIENTATIONS 4
#define MAP_BULLET_RANGE 1

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_POWER_UP 3
#define MAP_BULLET 3

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