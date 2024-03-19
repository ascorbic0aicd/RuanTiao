#pragma once
#include <cstdio>

#define LOG(...) //fprintf(stderr,__VA_ARGS__)
#define LOGERR(...) fprintf(stderr,__VA_ARGS__)
#define LOGLOC(...) //fprintf(stderr,__VA_ARGS__)
#include<list>
class Location;
typedef enum direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN,
    STAY
} direction;
typedef enum ROBOT_STATUS
{
    BROKEN,
    HAVE_GOOD,
    FREE,
    MOVING,
    JOGGING
} ROBOT_STATUS;
#define PATH list
#define PATH_TYPE Location
