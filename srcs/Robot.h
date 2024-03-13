#pragma once
typedef enum direction{RIGHT,LEFT,UP,DOWN} direction;
typedef enum ROBOT_STATUS{BROKEN,HAVE_GOOD,FREE,MOVING} ROBOT_STATUS;
#define ROBOT_NUM 10
extern int rbt_idx;
#include "Controler.h"
#include "cmath"
class Robot
{
    int x, y;
    ROBOT_STATUS status;
    direction step;
public:
    Robot() {}
    
    Robot(int startX, int startY):x(startX),y(startY),status(FREE), step(UP)
    {
        ctrls[x/DIV + 1][y/DIV + 1].addRobot(this);
    }
    inline int disTO(int x,int y){return abs(this->x - x) + abs(this->y - y);}
    inline ROBOT_STATUS getStatus(){return status;};
    void checkStatus(int x,int y,bool have_good, bool can_move);
};

extern Robot *robots[ROBOT_NUM + 5];