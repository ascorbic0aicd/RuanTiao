#pragma once
typedef enum direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
} direction;
typedef enum ROBOT_STATUS
{
    BROKEN,
    HAVE_GOOD, // go to berth
    FREE,
    MOVING // go to goods
} ROBOT_STATUS;
extern int rbt_idx;

#include "Controler.h"
#include "cmath"
#include "Cell.h"
#include "marcos.h"
#define ROBOT_NUM 10

class Robot
{
    int id;
    int x, y;
    ROBOT_STATUS status;
    direction step;

public:
    list<Point> path;

    Robot() {}

    Robot(int i, int startX, int startY) : id(i), x(startX), y(startY), status(FREE), step(UP)
    {
        ctrls[x / DIV + 1][y / DIV + 1].addRobot(this);
        LOG("addRobot %d %d %d\n",id, x / DIV + 1,y / DIV + 1);
    }
    inline int disTO(int x, int y) { return abs(this->x - x) + abs(this->y - y); }
    inline ROBOT_STATUS getStatus() { return status; };
    void checkStatus(int x, int y, bool have_good, bool can_move);
    list<Point> AStarPath(Point goal, Point dest);
    Point loc() { return Point(x, y); }
    void changeStatus(ROBOT_STATUS st){status=st;}
    void action();
    int ID(){return id;}
};

extern Robot *robots[ROBOT_NUM + 5];
